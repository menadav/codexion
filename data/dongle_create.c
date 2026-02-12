
#include "codexion.h"

int     create_dongle(t_dongle *dongles, long max)
{
        long i;

        i = 0;
        while (i < max)
        {
                if (pthread_mutex_init(&dongles[i].dongl, NULL) != 0)
                        return (1);
                dongles[i].available_cldw = 0;
                dongles[i].is_taken = 0;
                i++;
        }
        return (0);
}

t_dongle       *dongles_array(long max)
{
        t_dongle *dongles;

        dongles = (t_dongle *)malloc(sizeof(t_dongle) * max);
        if (!dongles)
                return (NULL);
        if(create_dongle(dongles, max) != 0)
                return(free(dongles), NULL);
        return (dongles);
}
