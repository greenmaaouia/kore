#include <time.h>
#include "kore.h"

#include "video_cache.h"
/*
static struct seqence * clean_cache(struct vsequence * s);
static int clean_sequence(struct vsequence * s);
static struct vsequence * get_sequence(struct vsequence * s, char *name);
static int addSequence(struct vsequence ** cache, struct vsequence *s);
static struct vsequence * initSequence(char * name, struct segment * data);
static struct vsequence * getSequence(struct vsequence *s, char *name);
static int cachesize(struct vsequence * cache);
static struct vsequence * get_sequence(struct vsequence * s, char *name);*/










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
addSequence(s);
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


int addSequence( struct vsequence *s){
	kore_log(LOG_ALERT,"adding sequence %s to cache at %lu\n",s->name,s->time);
	if(get_sequence(v_cache, s->name)==NULL){
		s->next=v_cache;
		s->time=time(NULL)+EXPIRE_TIME;
		v_cache=s;
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
