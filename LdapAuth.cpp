#include <iostream>
#include "LdapAuth.h"


using namespace std;


LdapAuth::LdapAuth()
{
	if ((ld = ldap_init(LDAP_HOST, LDAP_PORT)) == NULL)
	{
		mStatus=false;
	}
	{
		mStatus=true;
	}
}
LdapAuth::~LdapAuth() {
	ldap_unbind(ld);
}

bool LdapAuth::checkUser(string uid, string pwd)
{
	string dn = "uid=" + uid + ",ou=People,dc=technikum-wien,dc=at";

	int rc;
	rc = ldap_simple_bind_s(ld, dn.c_str(), pwd.c_str());

	if (rc != LDAP_SUCCESS)
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool LdapAuth::getStatus()
{

}