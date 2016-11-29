
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

struct myprogress {
    double lastruntime;
    CURL *curl;
};


void init_string( segment *s);
size_t writestringfunc(void *ptr, size_t size, size_t nmemb, struct segment *s);
static int xferinfo(void *p,
                    curl_off_t dltotal, curl_off_t dlnow,
                    curl_off_t ultotal, curl_off_t ulnow);
static int older_progress(void *p,
                          double dltotal, double dlnow,
                          double ultotal, double ulnow);


void curlgetsegment( char *url, struct segment * s);
static struct vsequence * v_cache=NULL;



void curlget( char *url, char **longString);

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
