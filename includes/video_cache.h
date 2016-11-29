
#define EXPIRE_TIME 60
typedef struct segment {
	char *ptr;
	size_t len;
} segment;

typedef struct vsequence{
	char * name;
	struct segment *data;
	time_t time;
	struct  vsequence *next;

}vsequence;



void *video_cache(void);

void                    vcacheclean(    void);
struct vsequence *      vcacheget(      char * name);
void                    vcacheadd(      struct vsequence *s);
int                     vcachelen(      void);





struct vsequence *      clean_cache(    struct vsequence * s);
int                     clean_sequence( struct vsequence * s);
struct vsequence *      get_sequence(   struct vsequence * s, char *name);
int                     addSequence(    struct vsequence *s);
struct vsequence *      initSequence(   char * name, struct segment * data);
int                     cachesize(      struct vsequence * cache);
