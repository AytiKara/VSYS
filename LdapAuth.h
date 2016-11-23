#include <iostream>
using namespace std;
#define LDAP_DEPRECATED 1
 #include <ldap.h>
//Defines
#define LDAP_HOST "ldap.technikum-wien.at"
#define LDAP_PORT 389


class LdapAuth
{
private:
	LDAP *ld;			/* LDAP resource handle */
	LDAPMessage *result, *e;	/* LDAP result handle */
	BerElement *ber;		/* array of attributes */
	char *attribute;
	char **vals;
	string dn;


	bool mStatus;

public:
	LdapAuth();
	~LdapAuth();

	bool checkUser(string pwd);
	bool getStatus();
	bool checkAllUser(string uid);
};
