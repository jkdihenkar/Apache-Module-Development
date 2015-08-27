#include "httpd.h"
#include "http_core.h"
#include "http_protocol.h"
#include "http_request.h"

//#include<stdio.h>

#include <curl/curl.h>

static void register_hooks(apr_pool_t *pool);
static int example_handler(request_rec *r);


module AP_MODULE_DECLARE_DATA   example_module =
{
    STANDARD20_MODULE_STUFF,
    NULL,            // Per-directory configuration handler
    NULL,            // Merge handler for per-directory configurations
    NULL,            // Per-server configuration handler
    NULL,            // Merge handler for per-server configurations
    NULL,            // Any directives we may have for httpd
    register_hooks   // Our hook registering function
};


static void register_hooks(apr_pool_t *pool)
{

    ap_hook_handler(example_handler, NULL, NULL, APR_HOOK_LAST);
}

static int printitem(void* rec, const char* key, const char* value) {
  /* rec is a userdata pointer.  We'll pass the request_rec in it */
  request_rec* r = rec ;
  ap_rprintf(r, "<tr><th scope=\"row\">%s</th><td>%s</td></tr>\n",
	ap_escape_html(r->pool, key), ap_escape_html(r->pool, value)) ;
  /* Zero would stop iterating; any other return value continues */
  return 1 ;
}

static void printtable(request_rec* r, apr_table_t* t,
	const char* caption, const char* keyhead, const char* valhead) {

  /* print a table header */
  ap_rprintf(r, "<table><caption>%s</caption><thead>"
	"<tr><th scope=\"col\">%s</th><th scope=\"col\">%s"
	"</th></tr></thead><tbody>", caption, keyhead, valhead) ;

  /* Print the data: apr_table_do iterates over entries with our callback */
  apr_table_do(printitem, r, t, NULL) ;

  /* and finish the table */
  ap_rputs("</tbody></table>\n", r) ;
}


static int example_handler(request_rec *r)
{
  char* postData="hello=hie";

  CURL *curl;
  
    if (!r->handler || strcmp(r->handler, "reqtest")) {
	return (DECLINED);
	}
	else {
	ap_set_content_type(r, "text/html");
    ap_rprintf(r,"The request handler is : %s <br>", r->handler);
	ap_rprintf(r,"The request method is : %s <br>", r->method);
	ap_rprintf(r,"The request filename is : %s <br>", r->filename);
	ap_rprintf(r,"The request args is : %s <br>", r->args);
	ap_rprintf(r,"The request headers_in params are as follows :  <br>");
	printtable(r,r->headers_in,"Request Headers", "Header", "Value");
	ap_rprintf(r,"The request User Agent is : %s <br>", r->useragent_ip);

  //say hello to outside server localhost:3000/getData.php

  curl = curl_easy_init();
  ap_rprintf(r,"Intialized Curl!<br>");
  if(curl) {
    ap_rprintf(r,"Curl Request Posting Started!<br>");

    curl_easy_setopt(curl, CURLOPT_URL,"http://google.com/search");
    //curl_easy_setopt(curl, CURLOPT_POST,1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS,postData);
    ap_rprintf(r,"%d",curl_easy_perform(curl));
    curl_easy_cleanup(curl);
  }



    return OK;
	}
}
