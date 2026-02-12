
#include "codexion.h"

long long get_time_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((long long)tv.tv_sec * 1000 + (long long)tv.tv_usec / 1000);
}

void release_dongle(t_dongle *dongle, t_data *data)
{
    pthread_mutex_lock(&dongle->dongl);
    dongle->available_cldw = get_time_ms() + data->dongle_cooldown;
    dongle->is_taken = 0;
    pthread_mutex_unlock(&dongle->dongl);
}

int can_take_dongle(t_dongle *dongle)
{
    long long now = get_time_ms();

    if (dongle->is_taken == 0 && now >= dongle->available_cldw)
        return (1);
    return (0);
}
