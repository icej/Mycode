/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2007 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id: header,v 1.16.2.1.2.1 2007/01/01 19:32:09 iliaa Exp $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_icej_tt.h"
#include "tcrdb.h"
#include "tculog.h"
#include "ttutil.h"

/* If you declare any globals in php_icej_tt.h uncomment this:*/
ZEND_DECLARE_MODULE_GLOBALS(icej_tt)


/* True global resources - no need for thread safety here */
static int le_icej_tt;

/* {{{ icej_tt_functions[]
 *
 * Every user visible function must have an entry in icej_tt_functions[].
 */
zend_function_entry icej_tt_functions[] = {
	PHP_FE(confirm_icej_tt_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE(icej_ttopen,	NULL)
	PHP_FE(icej_ttput,     NULL)
	PHP_FE(icej_ttget2,NULL)
	{NULL, NULL, NULL}	/* Must be the last line in icej_tt_functions[] */
};
/* }}} */

/* {{{ icej_tt_module_entry
 */
zend_module_entry icej_tt_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"icej_tt",
	icej_tt_functions,
	PHP_MINIT(icej_tt),
	PHP_MSHUTDOWN(icej_tt),
	PHP_RINIT(icej_tt),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(icej_tt),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(icej_tt),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_ICEJ_TT
ZEND_GET_MODULE(icej_tt)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
*/
/*
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("icej_tt.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_icej_tt_globals, icej_tt_globals)
    STD_PHP_INI_ENTRY("icej_tt.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_icej_tt_globals, icej_tt_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_icej_tt_init_globals
 */
/* Uncomment this function if you have INI entries*/
static void php_icej_tt_init_globals(zend_icej_tt_globals *icej_tt_globals)
{
	icej_tt_globals->global_value = 0;
	icej_tt_globals->global_string = NULL;
}

/* }}} */

static void icej_ttclose(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	TCRDB *ttlink = (TCRDB *)rsrc->ptr;
	if(tcrdbclose(ttlink))
	{
		php_printf("tt close true");
	}
	else
	{
		php_printf("tt close false");
	}

}
/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(icej_tt)
{
	//REGISTER_INI_ENTRIES();
	//ZEND_INIT_MODULE_GLOBALS(icej_tt, php_icej_tt_init_globals,NULL);
	le_icej_tt = zend_register_list_destructors_ex(icej_ttclose,NULL,"tt", module_number);
	return SUCCESS;
}


/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(icej_tt)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
//	UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(icej_tt)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(icej_tt)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(icej_tt)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "icej_tt support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	
	*/
//	DISPLAY_INI_ENTRIES();
}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_icej_tt_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_icej_tt_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "icej_tt", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/

/* {{{ proto resource icej_ttnew()
    */
PHP_FUNCTION(icej_ttopen)
{
	int ttlink_id = -1;
	int ksize,vsize,*port;
	zval *ttlinktmp = NULL;
	zval **arg1,**arg2;
	char *tmp,*tmpkey,*tmpvalue,*host;
	
	TCRDB *ttlink = tcrdbnew();
	php_printf("tt open\r\n");
	if (zend_get_parameters_ex(2, &arg1, &arg2) == FAILURE) 
	{
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid arguments");
		RETURN_FALSE;
	}	
	convert_to_string_ex(arg1);
	host = Z_STRVAL_PP(arg1);
	convert_to_string_ex(arg2);
	tmp = Z_STRVAL_PP(arg2);
	port = atoi(tmp);
	php_printf("host:%s,port:%d",host,port);
	if(tcrdbopen(ttlink,host,port))
	{
		php_printf("tt connect true\r\n");
	}
	else
	{
		php_printf("tt connect false\r\n");
	}
	ZEND_REGISTER_RESOURCE(return_value, ttlink, le_icej_tt);

}

PHP_FUNCTION(icej_ttput)
{
	zval **arg1,**arg2,**arg3;	
	int ksize,vsize;
	TCRDB *ttlink;
	char *ttkey,*ttvalue;
	if (zend_get_parameters_ex(3, &arg1, &arg2,&arg3) == FAILURE) 
	{
		 php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid arguments");
 		RETURN_FALSE;
	}
	convert_to_string_ex(arg2);
	ttkey = Z_STRVAL_PP(arg2);
	convert_to_string_ex(arg3);
	ttvalue = Z_STRVAL_PP(arg3);
	ZEND_FETCH_RESOURCE(ttlink, TCRDB *, arg1, -1, "tt",le_icej_tt);
	ksize = strlen(ttkey);
	vsize = strlen(ttvalue);
	if(tcrdbput(ttlink, ttkey,ksize,ttvalue,vsize))
	{
		php_printf("tt put true\r\n");
		RETURN_TRUE;
	}
	else
	{
		php_printf("tt put false\r\n");
		RETURN_FALSE;
	}
}

PHP_FUNCTION(icej_ttget2)
{
	zval **arg1,**arg2;
	TCRDB *ttlink;
	char *ttkey;
	if (zend_get_parameters_ex(2, &arg1, &arg2) == FAILURE)
	{
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid arguments");
		RETURN_FALSE;
	}
	convert_to_string_ex(arg2);
	ttkey = Z_STRVAL_PP(arg2);
	ZEND_FETCH_RESOURCE(ttlink, TCRDB *, arg1, -1, "tt",le_icej_tt);
	RETURN_STRING((ttlink,ttkey),0);
}
/* }}} */

/* {{{ proto bool icej_ttopen(resource ttlink)
   char host,int port) */
/*PHP_FUNCTION(icej_ttopen)
{
	int argc = ZEND_NUM_ARGS();
	int ttlink_id = -1;
	int ksize,vsize,*port;
	zval *ttlinktmp = NULL;
	zval **arg1,**arg2,**arg3;
	char *tmp,*tmpkey,*tmpvalue,*host;
	TCRDB *ttlink;
	php_printf("tt connect 1");
	if (zend_get_parameters_ex(3, &arg1, &arg2,&arg3) == FAILURE) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid arguments");
 		RETURN_FALSE;
	}
	convert_to_string_ex(arg2);
	host = Z_STRVAL_PP(arg2);
	convert_to_string_ex(arg3);
	tmp = Z_STRVAL_PP(arg3);
	port = atoi(tmp);
	php_printf("host:%s,port:%d",host,port);
	ZEND_FETCH_RESOURCE(ttlink, TCRDB *, arg1, ttlink_id, "standard-c-file",le_icej_tt);
	
	if (zend_parse_parameters(argc TSRMLS_CC, "r|s|l", &ttlinktmp,&host,&port) == FAILURE) 
		return;
	
	php_printf("tt connect 2");
	if (ttlinktmp) {
		ZEND_FETCH_RESOURCE(ttlink,TCRDB *, ttlinktmp, ttlink_id, "tt rescue", le_icej_tt);
	}

	php_printf("tt connect 3");
	if(tcrdbopen(ttlink,host,port))
	{
		php_printf("tt connect true");
	}
	else
	{
		php_printf("tt connect false");
	}
}
*/
/* }}} */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
