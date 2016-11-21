#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#define LDAP_DEPRECATED 1
#include <ldap.h>

#define LDAP_HOST "ldap.technikum-wien.at"
#define LDAP_PORT 389

using namespace std;
int main()
{
	
   LDAP *ld;			/* LDAP resource handle */
   int rc=0;

   /* setup LDAP connection */
   if ((ld=ldap_init(LDAP_HOST, LDAP_PORT)) == NULL)
   {
      perror("ldap_init failed");
      return EXIT_FAILURE;
   }

   printf("connected to LDAP server %s on port %d\n",LDAP_HOST,LDAP_PORT);

   /* anonymous bind */
   rc = ldap_simple_bind_s(ld,"uid=if15b076,ou=People,dc=technikum-wien,dc=at","");

   if (rc != LDAP_SUCCESS)
   {
      fprintf(stderr,"LDAP error: %s\n",ldap_err2string(rc));
      return EXIT_FAILURE;
   }
   else
   {
      printf("bind successful\n");
   }

   ldap_unbind(ld);
   return EXIT_SUCCESS;
}
   