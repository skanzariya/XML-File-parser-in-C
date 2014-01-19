/*
 * xmlConfigParser - a simple xml config file parser
 *
 * Copyright (C) 2009 Suresh Kanzariya <skanzariya@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.

	Usage: Parser config.xml
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

int main(int ac, char *av[]);
void ParseDoc(const char *);
void ParseElem(xmlDocPtr doc, xmlNodePtr cur);

/* A strict config file check required 
 * some predefined key
 * make it what you want ;)
 */
static char *key[] = {
"Location", /* 0 */
"IpAddress", /* 1 */
"Path", /* 2 */
"WhatElse", /* 3 */
"AddMore", /* 4 */
NULL
};

/*
 * A Global storage to store value from file
 */
const xmlChar *value[sizeof(key)/sizeof(key[0])];

void ParseElem(xmlDocPtr doc, xmlNodePtr cur)
{
	int  i;
	int *ptr = &i;
	*ptr = 0;
	xmlNodePtr childnode;
	xmlChar *data;
	if( cur->type == XML_ELEMENT_NODE ) {

		for(childnode=cur->children; childnode != NULL; childnode = childnode->next) {

				if( childnode->type == XML_ELEMENT_NODE ) {
					if ( !xmlStrcmp(childnode->name,(const xmlChar *)key[*ptr]) ) {
							data = xmlNodeGetContent(childnode);
							if (data && xmlStrlen(data) > 0){
								// fprintf(stderr,"Data %s %d %s %d\n",data,*ptr,key[*ptr],xmlStrlen(data));
								value[*ptr] = xmlStrdup(data) ;
							}else{
								fprintf(stderr,"Value required for '%s'\n",key[*ptr]);
								exit(1);
							}
								xmlFree(data);
								*ptr+=1;
					}else{
							fprintf(stderr,"Please check line '%s'\n",childnode->name);
							exit(1);
					}
				}
		}	
	}
	if( *ptr+1 != sizeof(key)/sizeof(key[0]) ){
		fprintf(stderr,"Please check config file\n");
		exit(0);
	}
}

void ParseDoc(const char *file)
{
	xmlDoc *doc;
	doc = xmlParseFile(file);
	if( doc == NULL ){
		fprintf(stderr,"Failed to Read file\n");
		exit(0);
	}
	xmlNode *cur;

	cur = xmlDocGetRootElement(doc);
	if( cur == NULL || !cur->name || (xmlStrcmp(cur->name,(const xmlChar *)"main")) ){
		fprintf(stderr,"Invalid xml File\n");
		xmlFreeDoc(doc);
		exit(0);
	}
	
	xmlNodePtr cur_node;
	
	/* We actually start reading from, where we found "data" in file */ 
	for(cur_node = cur->children; cur_node != NULL ; cur_node = cur_node->next){
			if( !xmlStrcmp(cur_node->name,(const xmlChar *)"data" ) ){
				ParseElem(doc, cur_node);
			}
	}

	xmlFreeDoc(doc);
	xmlCleanupParser();
}

int main(int ac, char *av[])
{
	if( av[1] == NULL ) {
		fprintf(stderr,"File name Req.\n");
		exit(0);
	}
	
	LIBXML_TEST_VERSION;
	xmlKeepBlanksDefault(0);
	ParseDoc(av[1]);

	/* Here you get value from config file ;)*/
	if( value[4] )	printf("value is %s\n", value[4]);
	fprintf(stderr,"\nHave a nice day ;)\n");
	return 0;
}
