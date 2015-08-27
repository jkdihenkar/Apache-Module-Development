#include<stdio.h>
#include<string.h>
#include<curl/curl.h>
#include<unistd.h>

int main() {

  //this example suppose that you have an echo server running that
  //logs the posted data on console
  //localhost/getData.php
  //find the sample server file getData.php

  char* postData="hello=hie";

  int parallel;

  CURLM *curlMultiple;
  CURL *curl[2];

  /*
    first making a single and assign handles
  */

  curl[0] = curl_easy_init();
  curl_easy_setopt(curl[0], CURLOPT_URL,"localhost:3000/getData.php");
  curl_easy_setopt(curl[0], CURLOPT_POSTFIELDS,postData);

  curl[1] = curl_easy_init();
  curl_easy_setopt(curl[0], CURLOPT_URL,"localhost:3000/getData0.php");
  curl_easy_setopt(curl[0], CURLOPT_POSTFIELDS,postData);


  /*
    POsting async multiple request
  */

  curlMultiple = curl_multi_init();
  curl_multi_add_handle(curlMultiple,curl[0]);
  curl_multi_add_handle(curlMultiple,curl[1]);

  curl_multi_perform(curlMultiple,&parallel);

  usleep(200);

  return 0;
}
