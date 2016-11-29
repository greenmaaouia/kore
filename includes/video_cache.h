
#define EXPIRE_TIME 60
struct segment {
	char *ptr;
	size_t len;
} ;

struct vsequence{
	char * name;
	struct segment *data;
	time_t time;
	struct  vsequence *next;

};



void *video_cache(void);
struct vsequence * clean_cache(struct vsequence * s);
int clean_sequence(struct vsequence * s);
struct vsequence * get_sequence(struct vsequence * s, char *name);
int addSequence(struct vsequence ** cache, struct vsequence *s);
struct vsequence * initSequence(char * name, struct segment * data);
int cachesize(struct vsequence * cache);
struct vsequence * get_sequence(struct vsequence * s, char *name);