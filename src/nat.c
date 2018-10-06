#include "../inc/includes.h"
#include "../inc/hashtable.h"

/* Used to add the key value to the hashtable */
void addKeyValueToHashTable (hashtable_t *hashtable, char *key, char *value) {
	//printf (" Log: Adding key<%s> value<%s>\r\n",key,value);
	return ht_set (hashtable, key, value);
}

/* Create and return the pointer to the hashtable */
hashtable_t * createHashTable (int size) {
	// print ("Log : Creating hash table... \r\n");
	return ht_create (size);
}

/* process through the NAT Rule file and build source->dest nat mappings */
void processNatRule(char *pathToRuleFile, hashtable_t *hashtable) {
	if (NULL == pathToRuleFile) {
		printf (" Rule File is NULL");
	}
	FILE *fp = NULL;
	char line[50];
	memset (&line, 0, 50);
	fp = fopen(pathToRuleFile,"r");
	while (fgets(line, 50, fp)) {
		if (strlen(line) <= 1) {
			continue;
		}

		line[strlen(line)-1] = '\0';
		char *source, *token, *dest;
		token = strtok(line,",");
		source = token;
		if (token) {
			token = strtok(NULL,",");
			dest = token;
		}
		addKeyValueToHashTable ( hashtable, source, dest);
	}
	fclose(fp);
	return;
}

/* Match the the input ip address across the rules that have
 * source as IpAddress:*
 */
char * MatchIpAddress (hashtable_t *hashtable, char *ipAddress) {
	char searchString[50];
	memset (searchString, 0, 50);
	sprintf(searchString,"%s:*",ipAddress);
	if (NULL != ht_get (hashtable, searchString)) {
		return ht_get (hashtable, searchString);
	}
	return NULL;
}

/* Match the input port number across the rules that have
 * *:port-number
 */
char * MatchPortNumber (hashtable_t *hashtable, char *port) {
	char searchString[50];
	memset (searchString, 0, 50);
	sprintf(searchString,"*:%s",port);
	if (NULL != ht_get (hashtable, searchString)) {
		return ht_get (hashtable, searchString);
	}
	return NULL;
}

/* Check the input IpAddress:port number across the Nat Rules
		Step 1: check if we have direct NAT rule
		Step 2: check if we have ip address part Match
		Step 3: check if we have match in port number part alone
		TODO: Can  match parts of the ip also.. which is not currently done
*/
char * checkMatch (hashtable_t *hashtable, char *source) {
	/* Check for a direct rule */
	if (NULL != ht_get( hashtable, source )) {
			return ht_get(hashtable, source);
	}

	/* Get the ipAddress and port and match them */
	char *input = (char *) malloc(strlen(source)+1);
	strncpy(input, source, strlen(source)+1);
	char *ipAddress, *port, *token;
	token = strtok(input,":");
	ipAddress = token;
	if (token) {
		token = strtok(NULL,":");
		port = token;
	}

	/* Match ip part alone */
	char *result = NULL;
	result = MatchIpAddress (hashtable, ipAddress);
	if (result != NULL) {
		free(input);
		return result;
	}

	/* Match port number alone */
	result = MatchPortNumber (hashtable, port);
	if (result != NULL) {
		free(input);
		return result;
	}
	free(input);
	return NULL;
}

/* Go through the flows(input), call checkMatch to match NAt Rule.
	 print the result.. Also store it in the output file
*/
void
translateFlows (hashtable_t *hashtable, char *inputFlowFile, char *outputFile){
	if ((hashtable == NULL) || (inputFlowFile == NULL) || (outputFile == NULL)) {
		printf (" Invalid arguments.. NULL");
		return;
	}

	FILE *writefp = NULL;
	FILE *readfp = NULL;

	char line[50];
	memset (&line, 0, 50);

	writefp = fopen(outputFile, "w+");
	readfp = fopen(inputFlowFile, "r");

	/* Go through every of the input flow file */
	while (fgets(line, 50, readfp)) {
		if (strlen(line) <= 1) {
			continue;
		}
		line[strlen(line)-1] = '\0';
		char *reply = NULL;
		/* Apply nat translation and get the output */
		reply = checkMatch (hashtable, line);
		if (reply == NULL) {
			printf ("No nat match for %s\r\n",line);
			fprintf (writefp,"No nat match for %s\r\n",line);
		}
		else {
			printf ("%s -> %s\r\n",line,reply);
			fprintf (writefp,"%s -> %s\r\n",line,reply);
		}
	}
	fclose(writefp);
	fclose(readfp);
	return;
}

int main( int argc, char **argv ) {
	if (argc < 4) {
		printf (" Incomplete Arguments: \r\n");
		printf (" Syntax:  nat <path to rule>  <path to input> <path to output> ");
		return 0;
	}

	char *natRuleFile = argv[1];
	char *inputFlowFile = argv[2];
	char *outputFile = argv[3];

	hashtable_t *hashtable = (hashtable_t *) createHashTable(65536);
	processNatRule (natRuleFile, hashtable);
	translateFlows (hashtable, inputFlowFile, outputFile);
	printf ("\r\nTranslation successfull. Refer to %s\r\n",outputFile);
	return 0;
}
