/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:25:47 by iammar            #+#    #+#             */
/*   Updated: 2025/06/24 18:13:28 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>

typedef struct s_philo
{
    pthread_t   thread;
    int     id;
    int     time_to_die;
    int     time_to_eat;
    int     time_to_sleep;
    int     number_eat;
    struct s_philo *next;
} t_philo;

t_philo *create_philosopher(int id, int  time_to_eat, int time_to_die, int time_to_sleep, int number_eat, void *(*routine) (void *))
{
    t_philo *philo;
    
    philo = malloc(sizeof(t_philo));
    pthread_create(&philo->thread, NULL, routine, "hello");
    philo->id = id;
    philo->time_to_die = time_to_die;
    philo->time_to_eat = time_to_eat;
    philo->number_eat = number_eat;
    philo->time_to_sleep = time_to_sleep;
    philo->next = NULL;
    
    return (philo);
}

void set_on_table(t_philo **head, t_philo *philo, int number)
{
    t_philo *curr;
    curr = *head;
    int     i = 1;
    if(!*head)
        *head = philo;
    else 
    {
        while(curr && curr->next)
        {
            i++;
            curr = curr->next;
        }
        curr->next = philo;
        if(i == number)
            curr->next->next = *head;
    }
}

void *routine(void *str)
{
    int i =0;
    while(i++ < 10)
    {
        printf("%s\n", (char *)str);
        usleep(500000);
    }
    return NULL;
}
int main(int ac , char **av)
{
    t_philo     *head;
    t_philo *philo;
    t_args t = {0};
    int i = 0;
    if(ac < 5 || ac > 6)
    {
        write(2,"invalid arguments\n",18);
    }
    else
    {
    t.number_of_philosophers = ft_atoi(av[1]);
    t.time_to_die = ft_atoi(av[2]);
    t.time_to_eat = ft_atoi(av[3]);
    t.time_to_sleep = ft_atoi(av[4]);
    if(av[5])
        t.number_of_times_each_philosopher_must_eat = ft_atoi(av[5]);
    }
    while(i < t.number_of_philosophers)
    {
        philo = create_philosopher(i, t.time_to_eat,t.time_to_die, t.time_to_sleep, t.number_of_times_each_philosopher_must_eat, routine);
        set_on_table(&head, philo, t.number_of_philosophers);
    }
    // pthread_join(*philo->thread, NULL);
    return 0;
}
