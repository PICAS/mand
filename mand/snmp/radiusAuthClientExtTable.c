/*
 * Note: this file originally auto-generated by mib2c using
 *       : mib2c.array-user.conf,v 5.26 2005/04/26 22:13:28 rstory Exp $
 *
 * $Id:$
 *
 *
 * For help understanding NET-SNMP in general, please check the 
 *     documentation and FAQ at:
 *
 *     http://www.net-snmp.org/
 *
 *
 * For help understanding this code, the agent and how it processes
 *     requests, please check the following references.
 *
 *     http://www.net-snmp.org/tutorial-5/
 *
 *
 * You can also join the #net-snmp channel on irc.freenode.net
 *     and ask for help there.
 *
 *
 * And if all else fails, send a detailed message to the developers
 *     describing the problem you are having to:
 *
 *    net-snmp-coders@lists.sourceforge.net
 *
 *
 */

#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/time.h>

#include "dm_token.h"
#include "dm_store.h"
#include "dm_index.h"

#define SDEBUG
#include "dm_assert.h"
#include "debug.h"

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

#include <net-snmp/library/snmp_assert.h>

#include "snmp_helper.h"
#include "radiusMIB_enums.h"
#include "radiusAuthClientExtTable.h"

static netsnmp_handler_registration *my_handler = NULL;
static netsnmp_table_array_callbacks cb;

static oid radiusAuthClientExtTable_oid[] = { radiusAuthClientExtTable_TABLE_OID };
static size_t radiusAuthClientExtTable_oid_len = OID_LENGTH(radiusAuthClientExtTable_oid);

static void initialize_table_radiusAuthClientExtTable(void);
static int radiusAuthClientExtTable_get_value(netsnmp_request_info *, netsnmp_index *, netsnmp_table_request_info *);

/**
 * Add a new client
 */
void
add_radiusAuthClientExtTable(dm_id id, struct dm_value_table *client)
{
	radiusAuthClientExtTable_context *row;

	ENTER();

	if (!my_handler) {
		EXIT();
		return;
	}

	row = malloc(sizeof(radiusAuthClientExtTable_context));
	if (!row) {
		EXIT();
		return;
	}

	row->radiusAuthServCounterDiscontinuity = ltime();
	row->index.len = 1;
	row->index.oids = &row->oid[0];
	row->oid[0] = id;
	row->radiusAuthClientExtIndex = id;
	row->client = client;
	debug("inserting row: %d (%p), client: %p", id, row, client);
	CONTAINER_INSERT(cb.container, row);
}

/**
 * Add a new client
 */
void
del_radiusAuthClientExtTable(dm_id id)
{
	netsnmp_index idx;
	oid soid[1];
	radiusAuthClientExtTable_context *row;

	idx.len = 1;
	idx.oids = &soid[0];
	soid[0] = id;

	row = CONTAINER_FIND(cb.container, &idx);
	if (row) {
		CONTAINER_REMOVE(cb.container, row);
		free(row);
	}
}

/************************************************************
 * Initializes the radiusAuthClientExtTable module
 */
void
init_radiusAuthClientExtTable()
{
	struct dm_instance *clnts;
	struct dm_instance_node *node;

	ENTER();

	initialize_table_radiusAuthClientExtTable();

	/** VAR: InternetGatewayDevice.X_TPLINO_NET_SessionControl.RadiusServer.Authentication.Client */
	clnts = dm_get_instance_ref_by_selector((dm_selector){ dm__InternetGatewayDevice,
				dm__IGD_X_TPLINO_NET_SessionControl,
				dm__IGD_SCG_RadiusServer,
				dm__IGD_SCG_RS_Authentication,
				dm__IGD_SCG_RS_Auth_Client, 0});

	if (!clnts) {
		EXIT();
		return;
	}

	for (node = dm_instance_first(clnts);
	     node != NULL;
	     node = dm_instance_next(clnts, node)) {
		/** VAR: InternetGatewayDevice.X_TPLINO_NET_SessionControl.RadiusServer.Authentication.Client.{i} */

		debug(": adding instance: %d (%p)", node->instance, DM_TABLE(node->table));
		add_radiusAuthClientExtTable(node->instance, DM_TABLE(node->table));
	}


	EXIT();
}

/************************************************************
 *
 * Initialize the radiusAuthClientExtTable table by defining its contents and how it's structured
 */
void
initialize_table_radiusAuthClientExtTable(void)
{
    netsnmp_table_registration_info *table_info;

    if (my_handler) {
        snmp_log(LOG_ERR,
                 "initialize_table_radiusAuthClientExtTable_handler called again\n");
        return;
    }

    memset(&cb, 0x00, sizeof(cb));

    /** create the table structure itself */
    table_info = SNMP_MALLOC_TYPEDEF(netsnmp_table_registration_info);

    my_handler =
        netsnmp_create_handler_registration("radiusAuthClientExtTable",
                                            netsnmp_table_array_helper_handler,
                                            radiusAuthClientExtTable_oid,
                                            radiusAuthClientExtTable_oid_len,
                                            HANDLER_CAN_RONLY);

    if (!my_handler || !table_info) {
        snmp_log(LOG_ERR, "malloc failed in "
                 "initialize_table_radiusAuthClientExtTable_handler\n");
        return; /** mallocs failed */
    }

    /***************************************************
     * Setting up the table's definition
     */
    /*
     * TODO: add any external indexes here.
     */

    /*
     * internal indexes
     */
        /** index: radiusAuthClientExtIndex */
    netsnmp_table_helper_add_index(table_info, ASN_INTEGER);

    table_info->min_column = radiusAuthClientExtTable_COL_MIN;
    table_info->max_column = radiusAuthClientExtTable_COL_MAX;

    /***************************************************
     * registering the table with the master agent
     */
    cb.get_value = radiusAuthClientExtTable_get_value;
    cb.container =
        netsnmp_container_find("radiusAuthClientExtTable_primary:"
                               "radiusAuthClientExtTable:"
                               "table_container");

    DEBUGMSGTL(("initialize_table_radiusAuthClientExtTable",
                "Registering table radiusAuthClientExtTable "
                "as a table array\n"));
    netsnmp_table_container_register(my_handler, table_info, &cb,
                                     cb.container, 1);
}

/************************************************************
 * radiusAuthClientExtTable_get_value
 *
 * This routine is called for get requests to copy the data
 * from the context to the varbind for the request. If the
 * context has been properly maintained, you don't need to
 * change in code in this fuction.
 */
int
radiusAuthClientExtTable_get_value(netsnmp_request_info *request,
                                   netsnmp_index * item,
                                   netsnmp_table_request_info *table_info)
{
	netsnmp_variable_list *var = request->requestvb;
	radiusAuthClientExtTable_context *ctx =
		(radiusAuthClientExtTable_context *) item;
	struct dm_value_table *stats;
	
	ENTER();

	if (!ctx->client) {
		snmp_log(LOG_ERR, "invalid server in "
			 "radiusAccServerExtTable_get_value\n");
		EXIT();
		return SNMP_ERR_GENERR;
	}

	/** VAR: InternetGatewayDevice.X_TPLINO_NET_SessionControl.RadiusServer.Authentication.Client.{i}.Stats */
	stats = dm_get_table_by_id(ctx->client, dm__IGD_SCG_RS_Auth_Clnt_i_Stats);

	switch (table_info->colnum) {
	case COLUMN_RADIUSAUTHCLIENTINETADDRESSTYPE:
		/** InetAddressType = ASN_INTEGER */
		snmp_set_var_typed_integer(var, ASN_INTEGER, INETADDRESSTYPE_IPV4);
		break;
		
	case COLUMN_RADIUSAUTHCLIENTINETADDRESS: {
		char hname[NI_MAXHOST];
		struct in_addr host;
		
		/** VAR: InternetGatewayDevice.X_TPLINO_NET_SessionControl.RadiusServer.Authentication.Client.{i}.Prefix */
		host = dm_get_ipv4_by_id(ctx->client, dm__IGD_SCG_RS_Auth_Clnt_i_Prefix);
		inet_ntop(AF_INET, &host, hname, sizeof(hname));
		
		/** InetAddress = ASN_OCTET_STR */
		snmp_set_var_typed_value(var, ASN_OCTET_STR, (unsigned char *)hname, strlen(hname));
		break;
	}
		
	case COLUMN_RADIUSAUTHCLIENTEXTID: {
		unsigned char *id;
		
		/** VAR: InternetGatewayDevice.X_TPLINO_NET_SessionControl.RadiusServer.Authentication.Client.{i}.Description */
		id = dm_get_string_by_id(ctx->client, dm__IGD_SCG_RS_Auth_Clnt_i_Description);
		if (!id || !*id)
			id = "";
		
		/** SnmpAdminString = ASN_OCTET_STR */
		snmp_set_var_typed_value(var, ASN_OCTET_STR, id, strlen(id));
		break;
	}

	case COLUMN_RADIUSAUTHSERVEXTACCESSREQUESTS:
		/** COUNTER = ASN_COUNTER */
		/** VAR: InternetGatewayDevice.X_TPLINO_NET_SessionControl.RadiusServer.Authentication.Client.{i}.Stats.AccessRequests */
		snmp_set_var_typed_integer(var, ASN_COUNTER, dm_get_uint_by_id(stats, dm__IGD_SCG_RS_Auth_Clnt_i_Stats_AccessRequests));
		break;
		
	case COLUMN_RADIUSAUTHSERVEXTDUPACCESSREQUESTS:
		/** COUNTER = ASN_COUNTER */
		/** VAR: InternetGatewayDevice.X_TPLINO_NET_SessionControl.RadiusServer.Authentication.Client.{i}.Stats.DupAccessRequests */
		snmp_set_var_typed_integer(var, ASN_COUNTER, dm_get_uint_by_id(stats, dm__IGD_SCG_RS_Auth_Clnt_i_Stats_DupAccessRequests));
		break;
		
	case COLUMN_RADIUSAUTHSERVEXTACCESSACCEPTS:
		/** COUNTER = ASN_COUNTER */
		/** VAR: InternetGatewayDevice.X_TPLINO_NET_SessionControl.RadiusServer.Authentication.Client.{i}.Stats.AccessAccepts */
		snmp_set_var_typed_integer(var, ASN_COUNTER, dm_get_uint_by_id(stats, dm__IGD_SCG_RS_Auth_Clnt_i_Stats_AccessAccepts));
		break;
		
	case COLUMN_RADIUSAUTHSERVEXTACCESSREJECTS:
		/** COUNTER = ASN_COUNTER */
		/** VAR: InternetGatewayDevice.X_TPLINO_NET_SessionControl.RadiusServer.Authentication.Client.{i}.Stats.AccessRejects */
		snmp_set_var_typed_integer(var, ASN_COUNTER, dm_get_uint_by_id(stats, dm__IGD_SCG_RS_Auth_Clnt_i_Stats_AccessRejects));
		break;
		
	case COLUMN_RADIUSAUTHSERVEXTACCESSCHALLENGES:
		/** COUNTER = ASN_COUNTER */
		/** VAR: InternetGatewayDevice.X_TPLINO_NET_SessionControl.RadiusServer.Authentication.Client.{i}.Stats.AccessChallenges */
		snmp_set_var_typed_integer(var, ASN_COUNTER, dm_get_uint_by_id(stats, dm__IGD_SCG_RS_Auth_Clnt_i_Stats_AccessChallenges));
		break;
		
	case COLUMN_RADIUSAUTHSERVEXTMALFORMEDACCESSREQUESTS:
		/** COUNTER = ASN_COUNTER */
		/** VAR: InternetGatewayDevice.X_TPLINO_NET_SessionControl.RadiusServer.Authentication.Client.{i}.Stats.MalformedRequests */
		snmp_set_var_typed_integer(var, ASN_COUNTER, dm_get_uint_by_id(stats, dm__IGD_SCG_RS_Auth_Clnt_i_Stats_MalformedRequests));
		break;
		
	case COLUMN_RADIUSAUTHSERVEXTBADAUTHENTICATORS:
		/** COUNTER = ASN_COUNTER */
		/** VAR: InternetGatewayDevice.X_TPLINO_NET_SessionControl.RadiusServer.Authentication.Client.{i}.Stats.BadAuthenticators */
		snmp_set_var_typed_integer(var, ASN_COUNTER, dm_get_uint_by_id(stats, dm__IGD_SCG_RS_Auth_Clnt_i_Stats_BadAuthenticators));
		break;
		
	case COLUMN_RADIUSAUTHSERVEXTPACKETSDROPPED:
		/** COUNTER = ASN_COUNTER */
		/** VAR: InternetGatewayDevice.X_TPLINO_NET_SessionControl.RadiusServer.Authentication.Client.{i}.Stats.PacketsDropped */
		snmp_set_var_typed_integer(var, ASN_COUNTER, dm_get_uint_by_id(stats, dm__IGD_SCG_RS_Auth_Clnt_i_Stats_PacketsDropped));
		break;
		
	case COLUMN_RADIUSAUTHSERVEXTUNKNOWNTYPES:
		/** COUNTER = ASN_COUNTER */
		/** VAR: InternetGatewayDevice.X_TPLINO_NET_SessionControl.RadiusServer.Authentication.Client.{i}.Stats.UnknownTypes */
		snmp_set_var_typed_integer(var, ASN_COUNTER, dm_get_uint_by_id(stats, dm__IGD_SCG_RS_Auth_Clnt_i_Stats_UnknownTypes));
		break;
		
	case COLUMN_RADIUSAUTHSERVCOUNTERDISCONTINUITY:
		/** TICKS = ASN_TIMETICKS */
		snmp_set_var_typed_integer(var, ASN_TIMETICKS, 
					   (ltime() - ctx->radiusAuthServCounterDiscontinuity) / 10);
		break;
		
	default:/** We shouldn't get here */
		snmp_log(LOG_ERR, "unknown column in "
			 "radiusAuthClientExtTable_get_value\n");
		EXIT();
		return SNMP_ERR_GENERR;
	}

	EXIT();
	return SNMP_ERR_NOERROR;
}
