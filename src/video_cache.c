#ifndef _GNU_SOURCE
    #define _GNU_SOURCE
#endif
#include <time.h>

#include <stdio.h>
#include "kore.h"

#include "video_cache.h"



void init_string( segment *s) {
    s->len = 0;
    s->ptr = (char *) malloc(s->len + 1);
    kore_log(LOG_ALERT,"init string\n");
    if (s->ptr == NULL) {
        fprintf(stderr, "kore_malloc() failed\n");
        exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';

}

size_t writestringfunc(void *ptr, size_t size, size_t nmemb, struct segment *s) {
    size_t new_len = s->len + size * nmemb;
    s->ptr = (char *) realloc(s->ptr, new_len + 1);
    if (s->ptr == NULL) {
        fprintf(stderr, "realloc() failed\n");
        exit(EXIT_FAILURE);
    }
    memcpy(s->ptr + s->len, ptr, size * nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;

    return size * nmemb;
}



static int xferinfo(void *p,
                    curl_off_t dltotal, curl_off_t dlnow,
                    curl_off_t ultotal, curl_off_t ulnow) {



    //  if (dlnow > STOP_DOWNLOAD_AFTER_THIS_MANY_BYTES)
    //    return 1;
    return 0;
}

static int older_progress(void *p,
                          double dltotal, double dlnow,
                          double ultotal, double ulnow) {
    return xferinfo(p,
                    (curl_off_t) dltotal,
                    (curl_off_t) dlnow,
                    (curl_off_t) ultotal,
                    (curl_off_t) ulnow);
}

void curlgetsegment( char *url, struct segment * s) {

    CURL *curl;
    kore_log(LOG_ALERT,"curl doing job\n");
    struct myprogress prog;


    curl = curl_easy_init();
    if (curl) {

        // struct segment s;
        init_string(s);

        curl_easy_setopt(curl, CURLOPT_URL,
                         url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writestringfunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, s);
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, older_progress);
        curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &prog);

#if LIBCURL_VERSION_NUM >= 0x072000
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, xferinfo);
        curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &prog);
#endif

        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
         curl_easy_perform(curl);


        curl_easy_cleanup(curl);
        kore_log(LOG_ALERT,"done %lu\n",s->len);

    }
}


void curlget( char *url, char **longString) {

    CURL *curl;

    struct myprogress prog;


    curl = curl_easy_init();
    if (curl) {

        struct segment s;
        init_string(&s);

        curl_easy_setopt(curl, CURLOPT_URL,
                         url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writestringfunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, older_progress);
        curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &prog);

#if LIBCURL_VERSION_NUM >= 0x072000
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, xferinfo);
        curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &prog);
#endif

        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
        curl_easy_perform(curl);

        int i=asprintf(longString, "%s", s.ptr);
        printf("%d",i);
        free(s.ptr);

        curl_easy_cleanup(curl);

    }
}/////////////////////////////////////////////////////////////////////////////////


 void 		*video_cache(void)
{
	return (void*) v_cache;
}

int vcachelen(void){
    return cachesize(v_cache);
}

void vcacheclean(void){
    clean_cache(v_cache);
}

struct vsequence * vcacheget(char * name){
    return get_sequence(v_cache,name);
}

void vcacheadd( struct vsequence *s){
//addSequence(s);
}



int cachesize(struct vsequence * cache){
	if(cache==NULL) return 0;
	return cachesize(cache->next)+1;



}

struct vsequence * initSequence(char * name, struct segment * data){
	kore_log(LOG_ALERT,"initialising sequence %s of %lu byte\n",name,data->len);
	name[strlen(name)-1]='\0';
	struct vsequence * sequence1;
	sequence1=(struct vsequence *) malloc(sizeof(struct vsequence));
	sequence1->data=data;
	sequence1->next=NULL;
	sequence1->name=(char*)malloc(sizeof(char*)*strlen(name));
            memcpy(sequence1->name,name,strlen(name));
	sequence1->time=time(NULL);
	kore_log(LOG_ALERT,"initialised sequence %s of %lu byte\n",sequence1->name,sequence1->data->len);
	return sequence1;
}


int addSequence( struct vsequence *s, struct vsequence **m_cache){
	kore_log(LOG_ALERT,"adding sequence %s to cache at %lu\n",s->name,s->time);
	if(get_sequence(*m_cache, s->name)==NULL){
		s->next=*m_cache;
		s->time=time(NULL)+EXPIRE_TIME;
		*m_cache=s;
		kore_log(LOG_ALERT,"sequence  cached now\n");
		return 0;
	}
	kore_log(LOG_ALERT,"sequence already cached\n");
	return 1;

}

struct vsequence * get_sequence(struct vsequence * s, char *name){
	if(s==NULL) return NULL;
	// kore_log(LOG_ALERT,"visiting %s :%lu",s->name,s->time);

	return (strcmp(s->name,name)==0)?s:get_sequence(s->next,name);

}



int clean_sequence(struct vsequence * s){
	free(s->data->ptr);
	s->data->ptr = NULL;
	free(s->data);
	s->data = NULL;
	kore_log(LOG_ALERT, "name: %s , %lu\n",s->name,s->time);
	free(s->name);
	s->name = NULL;
	return 0;
}


 struct vsequence * clean_cache(struct vsequence * s)
{

	if(s == NULL)
		return NULL;
	kore_log(LOG_ALERT,"cleaning cache %s %lu @ %lu\n",s->name,s->time,s);
	if (s->time< time(NULL))
	{
		struct vsequence * tmp = s->next;
		clean_sequence(s);

		tmp = clean_cache(tmp);
		return tmp;
	}    else    {

		s->next = clean_cache(s->next);
		return s;
	}
}
