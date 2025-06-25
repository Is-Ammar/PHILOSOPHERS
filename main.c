/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:25:47 by iammar            #+#    #+#             */
/*   Updated: 2025/06/25 18:37:40 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

typedef struct s_args
{
    int number_of_philosophers;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int number_of_times_each_philosopher_must_eat;
    pthread_mutex_t *forks;
    pthread_mutex_t print_mutex;
    int simulation_stop;
    long long start_time;
} t_args;

typedef struct s_philo
{
    pthread_t thread;
    int id;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int number_eat;
    int meals_eaten;
    long long last_meal_time;
    int left_fork;
    int right_fork;
    t_args *args;
    struct s_philo *next;
} t_philo;

long long get_time_ms(void)
{
    struct timeval tv;
    
    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void print_state(t_philo *philo, char *state)
{
    pthread_mutex_lock(&philo->args->print_mutex);
    if (!philo->args->simulation_stop)
    {
        long long current_time = get_time_ms() - philo->args->start_time;
        printf("%lld %d %s\n", current_time, philo->id + 1, state);
    }
    pthread_mutex_unlock(&philo->args->print_mutex);
}

void precise_sleep(long long ms)
{
    long long start_time = get_time_ms();
    long long end_time = start_time + ms;
    
    while (get_time_ms() < end_time)
        usleep(100);
}

void take_forks(t_philo *philo)
{
    if (philo->left_fork < philo->right_fork)
    {
        pthread_mutex_lock(&philo->args->forks[philo->left_fork]);
        print_state(philo, "has taken a fork");
        pthread_mutex_lock(&philo->args->forks[philo->right_fork]);
        print_state(philo, "has taken a fork");
    }
    else
    {
        pthread_mutex_lock(&philo->args->forks[philo->right_fork]);
        print_state(philo, "has taken a fork");
        pthread_mutex_lock(&philo->args->forks[philo->left_fork]);
        print_state(philo, "has taken a fork");
    }
}

void put_down_forks(t_philo *philo)
{
    pthread_mutex_unlock(&philo->args->forks[philo->left_fork]);
    pthread_mutex_unlock(&philo->args->forks[philo->right_fork]);
}

void eat(t_philo *philo)
{
    print_state(philo, "is eating");
    philo->last_meal_time = get_time_ms();
    precise_sleep(philo->time_to_eat);
    philo->meals_eaten++;
}

void *routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    
    usleep((philo->id % 3) * 1000);
    
    philo->last_meal_time = get_time_ms();
    
    while (!philo->args->simulation_stop && 
          (philo->number_eat == 0 || philo->meals_eaten < philo->number_eat))
    {
        print_state(philo, "is thinking");
        
        take_forks(philo);
        
        if (!philo->args->simulation_stop)
        {
            eat(philo);
            
            put_down_forks(philo);
            
            if (!philo->args->simulation_stop)
            {
                print_state(philo, "is sleeping");
                precise_sleep(philo->time_to_sleep);
            }
        }
        else
        {
            put_down_forks(philo);
        }
    }
    return NULL;
}

void *monitor_routine(void *arg)
{
    t_philo *head = (t_philo *)arg;
    t_philo *curr;
    int all_philosophers_eaten = 0;
    
    while (!all_philosophers_eaten)
    {
        curr = head;
        all_philosophers_eaten = 1;
        
        do {
            if (get_time_ms() - curr->last_meal_time > curr->time_to_die)
            {
                pthread_mutex_lock(&curr->args->print_mutex);
                if (!curr->args->simulation_stop)
                {
                    printf("%lld %d died\n", 
                        get_time_ms() - curr->args->start_time, 
                        curr->id + 1);
                    curr->args->simulation_stop = 1;
                }
                pthread_mutex_unlock(&curr->args->print_mutex);
                return NULL;
            }
            
            if (curr->number_eat > 0 && curr->meals_eaten < curr->number_eat)
                all_philosophers_eaten = 0;
                
            curr = curr->next;
            
            usleep(500);
            
        } while (curr != head && !curr->args->simulation_stop);
        
        if (all_philosophers_eaten)
        {
            pthread_mutex_lock(&curr->args->print_mutex);
            curr->args->simulation_stop = 1;
            pthread_mutex_unlock(&curr->args->print_mutex);
        }
    }
    return NULL;
}

t_philo *create_philosopher(int id, t_args *args)
{
    t_philo *philo;
    
    philo = malloc(sizeof(t_philo));
    if (!philo)
        return NULL;
    
    philo->id = id;
    philo->time_to_die = args->time_to_die;
    philo->time_to_eat = args->time_to_eat;
    philo->time_to_sleep = args->time_to_sleep;
    philo->number_eat = args->number_of_times_each_philosopher_must_eat;
    philo->meals_eaten = 0;
    philo->last_meal_time = get_time_ms();
    philo->args = args;

    philo->left_fork = id;
    philo->right_fork = (id + 1) % args->number_of_philosophers;
    philo->next = NULL;
    
    return philo;
}

void set_on_table(t_philo **head, t_philo *philo)
{
    t_philo *curr;

    if (!*head)
    {
        *head = philo;
        (*head)->next = *head;
        return;
    }
    
    curr = *head;
    while (curr->next != *head)
        curr = curr->next;
    
    curr->next = philo;
    philo->next = *head;
}

int init_forks(t_args *args)
{
    int i;
    
    args->forks = malloc(sizeof(pthread_mutex_t) * args->number_of_philosophers);
    if (!args->forks)
        return 0;
        
    for (i = 0; i < args->number_of_philosophers; i++)
        if (pthread_mutex_init(&args->forks[i], NULL) != 0)
            return 0;
            
    return pthread_mutex_init(&args->print_mutex, NULL) == 0;
}

void cleanup(t_args *args, t_philo *head)
{
    int i;
    t_philo *curr, *next;
    
    for (i = 0; i < args->number_of_philosophers; i++)
        pthread_mutex_destroy(&args->forks[i]);
    pthread_mutex_destroy(&args->print_mutex);
    
    free(args->forks);
    
    if (head)
    {
        curr = head;
        do {
            next = curr->next;
            free(curr);
            curr = next;
        } while (curr != head);
    }
}

int main(int ac, char **av)
{
    t_philo *head = NULL;
    t_philo *philo;
    t_args args = {0};
    pthread_t monitor;
    int i = 0;

    if (ac < 5 || ac > 6)
    {
        write(2, "invalid arguments\n", 18);
        exit(1);
    }
    else
    {
        args.number_of_philosophers = ft_atoi(av[1]);
        args.time_to_die = ft_atoi(av[2]);
        args.time_to_eat = ft_atoi(av[3]);
        args.time_to_sleep = ft_atoi(av[4]);
        if (ac == 6)
            args.number_of_times_each_philosopher_must_eat = ft_atoi(av[5]);
    }

    args.simulation_stop = 0;
    
    if (!init_forks(&args))
    {
        write(2, "Failed to initialize mutexes\n", 29);
        exit(1);
    }
    
    args.start_time = get_time_ms();

    while (i < args.number_of_philosophers)
    {
        philo = create_philosopher(i, &args);
        if (!philo)
        {
            write(2, "Failed to create philosopher\n", 28);
            cleanup(&args, head);
            exit(1);
        }
        set_on_table(&head, philo);
        i++;
    }
    
    philo = head;
    i = 0;
    while (i < args.number_of_philosophers)
    {
        if (pthread_create(&philo->thread, NULL, routine, philo) != 0)
        {
            write(2, "Failed to create thread\n", 24);
            args.simulation_stop = 1;
            cleanup(&args, head);
            exit(1);
        }
        philo = philo->next;
        i++;
    }
    
    if (pthread_create(&monitor, NULL, monitor_routine, head) != 0)
    {
        write(2, "Failed to create monitor thread\n", 31);
        args.simulation_stop = 1;
        cleanup(&args, head);
        exit(1);
    }
    
    pthread_join(monitor, NULL);
    
    philo = head;
    for (i = 0; i < args.number_of_philosophers; i++)
    {
        pthread_detach(philo->thread);
        philo = philo->next;
    }
    
    cleanup(&args, head);
    
    return 0;
}