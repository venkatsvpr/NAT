# NAT
Implemented a NAT, that reads from a rule file and peforms Network Address Translation on the flow file and generates the output.

Design:
=======
The code is structured into two phases,
  1) Read through the NAT Rule
  2) Read the input flow and perform translation

A hash-table is used to store and retrieve the NAT-Rule. The rationale behind choose hash-table is to give a O(1) lookup. 
The hash-table implementation was obtained over the internet and reference has been given.
HashTable source: https://gist.github.com/tonious/1377667/104d94bb2f5b87a09dfd8cb181e302f19e46e1aa


Future scope:
============
The Current code matches, IpAddress+Port, AnyIp+Port, Ip+AnyPort scenario.
This does support matching the Network or Host portion of the IpAddress.. Like Matching A.B.XXX.XXX for NAT Traversal.
(We could also use a Trie to store the same and do a lookup based on the lenght of the input)


Optimizations (ToDo):
=====================
- More error sceanrios and edge cases need to be tested and handled

Testing:
========
Manual Testing was some flows and rules.


Functions:
==========

void addKeyValueToHashTable (hashtable_t *hashtable, char *key, char *value)
Adds <key,value> pair to hashtable

hashtable_t * createHashTable (int size)
Create and return the hashtable

void processNatRule(char *pathToRuleFile, hashtable_t *hashtable)
Process the rules and populates the hashtable

char * MatchIpAddress (hashtable_t *hashtable, char *ipAddress)
Match IpAddress:AnyPort scenario

char * MatchPortNumber (hashtable_t *hashtable, char *port)
Match the AnyIpAddress:port scenario

char * checkMatch (hashtable_t *hashtable, char *source)
Check the whole of IpAdress:Port (or) IpAddress:AnyPort (or) AnyIpAddress:Port

translateFlows (hashtable_t *hashtable, char *inputFlowFile, char *outputFile)
Translate the flows in the inputfile to output file using the rules in the hashtable.

main function
invokes other functions.


HashTable APIs:
==============
Create hashtable - hashtable_t *ht_create( int size )
Add key,value to hashtable - void ht_set( hashtable_t *hashtable, char *key, char *value ) {
Get value for key from hashtable - char *ht_get( hashtable_t *hashtable, char *key ) {






