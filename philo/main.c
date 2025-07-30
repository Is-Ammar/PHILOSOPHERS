/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:25:47 by iammar            #+#    #+#             */
/*   Updated: 2025/07/26 17:30:26 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_args(t_args *args, char **av)
{
	args->number_of_philosophers = ft_atoi(av[1]);
	args->time_to_die = ft_atoi(av[2]);
	args->time_to_eat = ft_atoi(av[3]);
	args->time_to_sleep = ft_atoi(av[4]);
	if (args->number_of_philosophers <= 0 || args->time_to_die <= 0
		|| args->time_to_eat < 0 || args->time_to_sleep < 0)
	{
		write(2, "invalid argument values\n", 24);
		return (1);
	}
	return (0);
}

int	parse(t_args *args, int ac, char **av)
{
	if (ac < 5 || ac > 6)
	{
		write(2, "invalid arguments\n", 18);
		return (1);
	}
	if (get_args(args, av))
		return (1);
	if (ac == 6)
	{
		args->number_of_eats = ft_atoi(av[5]);
		if (args->number_of_eats <= 0)
		{
			write(2, "invalid number of eats\n", 23);
			return (1);
		}
	}
	else
		args->number_of_eats = -1;
	return (0);
}

int	one_philo(t_args *args)
{
	long long	current_time;

	current_time = get_timestamp();
	printf("%lld 1 has taken a fork\n", current_time - args->start_time);
	usleep(args->time_to_die);
	printf("%d 1 died\n", args->time_to_die);
	return (1);
}

int	create(t_philo *philo, t_philo **head, t_args *args)
{
	t_philo	*current;
	int		i;

	i = 1;
	if (args->number_of_philosophers == 1)
	{
		args->start_time = get_timestamp();
		return (one_philo(args));
	}
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
	return (0);
}

int	main(int ac, char **av)
{
	t_philo		*head;
	t_args		args;
	pthread_t	monitor_thread;
	t_philo		*philo;

	args = (t_args){0};
	philo = NULL;
	head = NULL;
	if (parse(&args, ac, av))
		return (1);
	init_mutex(&args);
	if (create(philo, &head, &args))
		return (0);
	args.start_time = get_timestamp();
	pthread_mutex_lock(&args.lock);
	args.simulation_running = 1;
	pthread_mutex_unlock(&args.lock);
	pthread_create(&monitor_thread, NULL, monitor, &args);
	join_threads(head, args.number_of_philosophers);
	pthread_join(monitor_thread, NULL);
	clear_philo(head, args.number_of_philosophers);
	destroy_mutex(&args);
	return (0);
}
