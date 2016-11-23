#include <iostream>
#include "LdapAuth.h"
#define SEARCHBASE "dc=technikum-wien,dc=at"
#define SCOPE LDAP_SCOPE_SUBTREE
#define FILTER "(uid=if15b*)"
#include <cstdlib>
using namespace std;


LdapAuth::LdapAuth()
{
	if ((ld = ldap_init(LDAP_HOST, LDAP_PORT)) == NULL)
	{
		mStatus = false;
	}
	{
		mStatus = true;
	}
}
LdapAuth::~LdapAuth() {
	ldap_unbind(ld);
}

bool LdapAuth::checkUser(string pwd)
{

	// string dn = "uid=" + uid + ",ou=People,dc=technikum-wien,dc=at";

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
bool LdapAuth::checkAllUser(string uid)
{
	int rc;
	char *attribs[3];		/* attribute array for search */

	attribs[0] = strdup("uid");		/* return uid and cn of entries */
	attribs[1] = strdup("cn");
	attribs[2] = NULL;
	rc = ldap_simple_bind_s(ld, NULL, NULL);

	if (rc != LDAP_SUCCESS)
	{
		return false;
	}

	/* perform ldap search */
	rc = ldap_search_s(ld, SEARCHBASE, SCOPE, FILTER, attribs, 0, &result);

	if (rc != LDAP_SUCCESS)
	{
		return false;
	}

	for (e = ldap_first_entry(ld, result); e != NULL; e = ldap_next_entry(ld, e))
	{
		dn = ldap_get_dn(ld, e );
		/* Now print the attributes and values of each found entry */

		for (attribute = ldap_first_attribute(ld, e, &ber); attribute != NULL; attribute = ldap_next_attribute(ld, e, ber))
		{
			if ((vals = ldap_get_values(ld, e, attribute)) != NULL)
			{
				for (int i = 0; vals[i] != NULL; i++)
				{
					string attr = attribute;
					if (attr == "uid")
					{
						string valUid = vals[i];
						if (valUid == uid)
							return true;
					}

				}

				/* free memory used to store the values of the attribute */
				ldap_value_free(vals);
			}
			/* free memory used to store the attribute */
			ldap_memfree(attribute);
		}
		/* free memory used to store the value structure */
		if (ber != NULL) ber_free(ber, 0);

		// printf("\n");
	}

	/* free memory used for result */
	ldap_msgfree(result);
	free(attribs[0]);
	free(attribs[1]);
	return false;

}
bool LdapAuth::getStatus()
{

}