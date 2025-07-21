/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:25:47 by iammar            #+#    #+#             */
/*   Updated: 2025/07/21 20:28:54 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	clear_philo(t_philo *philo, int i)
{
	t_philo	*nxt;

	while (philo && i-- > 0)
	{
		nxt = philo->next;
		if (philo)
		{
			free(philo);
			philo = NULL;
		}
		philo = nxt;
	}
}

int	parse(t_args *args, int ac, char **av)
{
	if (ac < 5 || ac > 6)
	{
		write(2, "invalid arguments\n", 18);
		return (1);
	}
	else
	{
		args->number_of_philosophers = ft_atoi(av[1]);
		args->time_to_die = ft_atoi(av[2]);
		args->time_to_eat = ft_atoi(av[3]);
		args->time_to_sleep = ft_atoi(av[4]);
		if (ac == 6)
			args->number_of_eats = ft_atoi(av[5]);
		else
			args->number_of_eats = -1;
	}
	args->all_ate = 0;
	args->simulation_running = 0;
    return(0);
}

void	create(t_philo *philo, t_philo **head, t_args *args)
{
	t_philo	*current;
	int		i;

	i = 1;
	while (i <= args->number_of_philosophers)
	{
		philo = create_philosopher(i, args);
		set_on_table(head, philo);
		i++;
	}
	args->philosophers_head = *head;
	current = *head;
	i = args->number_of_philosophers;
	while (i > 0)
	{
		pthread_create(&current->thread, NULL, routine, current);
		current = current->next;
		i--;
	}
}

void	join_threads(t_philo *head, int count)
{
	t_philo	*current;

	current = head;
	while (count > 0 && current)
	{
		pthread_join(current->thread, NULL);
		current = current->next;
		count--;
	}
}

int	main(int ac, char **av)
{
	t_philo		*head;
	t_philo		*philo;
	t_args		args;
	int			i;
	pthread_t	monitor_thread;

	head = NULL;
	philo = NULL;
	args = (t_args){0};
	i = args.number_of_philosophers;
	if (parse(&args, ac, av))
        return 1;
	pthread_mutex_init(&args.mutex, NULL);
	pthread_mutex_init(&args.print_mutex, NULL);
	pthread_mutex_init(&args.lock, NULL);
	create(philo, &head, &args);
	args.start_time = get_timestamp();
	pthread_mutex_lock(&args.lock);
	args.simulation_running = 1;
	pthread_mutex_unlock(&args.lock);
	pthread_create(&monitor_thread, NULL, monitor, &args);
	join_threads(head, args.number_of_philosophers);
	pthread_join(monitor_thread, NULL);
	clear_philo(head, args.number_of_philosophers);
	return (0);
}
