
#include "codexion.h"

void create_coder(t_data *data, t_coder *coder, int i)
{
        coder->id = i;
        coder->compiles_done = 0;
        coder->last_compile_start = 0;
        coder->data = data;
        if (i == 0)
                coder->l_dg = &data->dongles[data->number_of_coders - 1];
        else
                coder->l_dg = &data->dongles[i - 1];
        coder->r_dg = &data->dongles[i];
}

t_coder *virtual_coders(t_data *data)
{
        int i;
        t_coder *coders;

        i = 0;
        coders = malloc(sizeof(t_coder) * data->number_of_coders);
        if (!coders)
                return (NULL);
        while (i < data->number_of_coders)
        {
                create_coder(data, &coders[i], i);
                i++;
        }
        return (coders);
}
