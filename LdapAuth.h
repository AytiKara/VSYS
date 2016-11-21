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
	LDAP *ld;
	bool mStatus;
public:
	LdapAuth();
	~LdapAuth();

	bool checkUser(string uid, string pwd);
	bool getStatus();
};
