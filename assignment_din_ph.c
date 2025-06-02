/*
 * =====================================================================================
 *
 *       Filename:  din_ph.c
 *
 *    Description: This file implements the Dining Philosopher Problem 
 *
 *        Version:  1.0
 *        Created:  01/30/2021 03:18:59 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ABHISHEK SAGAR (), sachinites@gmail.com
 *   Organization:  Juniper Networks
 * 		Modified by:  Howie Chang
 *   	Modified on:  2025/6/2
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include "din_ph.h"


#define N_PHILOSOPHER	5

static phil_t phil[N_PHILOSOPHER];
static spoon_t spoon[N_PHILOSOPHER];

static spoon_t *phil_get_right_spoon(phil_t *phil) {

	int phil_id = phil->phil_id;

	if (phil_id == 0) {
		return &spoon[N_PHILOSOPHER -1];
	}
	return &spoon[phil_id - 1];
}

static spoon_t *phil_get_left_spoon(phil_t *phil) {

	return &spoon[phil->phil_id];
}

void
phil_eat(phil_t *phil) {

	spoon_t *left_spoon = phil_get_left_spoon(phil);
	spoon_t *right_spoon = phil_get_right_spoon(phil);

	/*
 	 *  Check condition that Phil is eating with right set of
 	 *  spoons
 	 */
	assert(left_spoon->phil == phil);
	assert(right_spoon->phil == phil);
	assert(left_spoon->is_used == true);
	assert(right_spoon->is_used == true);
	phil->eat_count++;
	printf("Phil %d eats with spoon [%d, %d] for %d times\n",
		phil->phil_id, left_spoon->spoon_id, right_spoon->spoon_id,
		phil->eat_count);
	sleep(1); /* let the philosopher enjoy his turn of eating for 1 sec */
}

void
philosopher_release_both_spoons(phil_t *phil) {

	spoon_t *left_spoon  = phil_get_left_spoon(phil);
	spoon_t *right_spoon = phil_get_right_spoon(phil);

	pthread_mutex_lock(&left_spoon->mutex);
	pthread_mutex_lock(&right_spoon->mutex);

	assert(left_spoon->is_used == true);
	assert(left_spoon->phil == phil);
	assert(right_spoon->is_used == true);
	assert(right_spoon->phil == phil);
	left_spoon -> phil = NULL;
	left_spoon -> is_used = false;

	pthread_cond_signal(&left_spoon->cv);
	pthread_mutex_unlock(&left_spoon->mutex);

	right_spoon -> phil = NULL;
	right_spoon -> is_used = false;

	pthread_cond_signal(&right_spoon->cv);
	pthread_mutex_unlock(&right_spoon->mutex);

}

bool
philosopher_get_access_both_spoons(phil_t *phil) {

	spoon_t *left_spoon  = phil_get_left_spoon(phil);
	spoon_t *right_spoon = phil_get_right_spoon(phil);

	pthread_mutex_lock(&left_spoon -> mutex);

	while(left_spoon -> is_used && left_spoon ->phil != phil){
		pthread_cond_wait(&left_spoon->cv, &left_spoon->mutex);
	}
	
	left_spoon->is_used = true;
	left_spoon->phil = phil;

	pthread_mutex_unlock(&left_spoon->mutex);

	pthread_mutex_lock(&right_spoon->mutex);

	if(right_spoon -> is_used == false){
		right_spoon -> is_used = true;
		right_spoon->phil = phil;
		pthread_mutex_unlock(&right_spoon->mutex);
		return true;
	}else{
		if(right_spoon->phil != phil){
			pthread_mutex_lock(&left_spoon->mutex);
			assert(left_spoon->is_used == true);
			assert(left_spoon->phil == phil);
			left_spoon->is_used = false;
			left_spoon->phil = NULL;
			pthread_mutex_unlock(&left_spoon->mutex);
			pthread_mutex_unlock(&right_spoon->mutex);
			return false;
		}else{
			pthread_mutex_unlock(&right_spoon->mutex);
			return true;
		}
	}

	assert(0);
	return false;
}

void *
philosopher_fn(void *arg) {
	phil_t *phil = (phil_t *)arg;
	/* Start implementation of core logic from here */
	while(1){
		if(philosopher_get_access_both_spoons(phil)){
			phil_eat(phil);
			philosopher_release_both_spoons(phil);
			sleep(1);
		}
	}
}

int
main(int argc, char **argv) {

	int i = 0;
	pthread_attr_t attr;

	/* Initialize all spoons */
	for ( i = 0; i < N_PHILOSOPHER; i++ ) {

		spoon[i].spoon_id = i;
		spoon[i].is_used = false;
		spoon[i].phil = NULL;
		pthread_mutex_init(&spoon[i].mutex, NULL);
		pthread_cond_init(&spoon[i].cv, NULL);
	}

	/*  Default attributes of all Philosopher threads */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	/* create philosopher threads */
	for ( i = 0; i < N_PHILOSOPHER; i++) {
		phil[i].phil_id = i;
		phil[i].eat_count = 0;
		pthread_create(&phil[i].thread_handle, &attr, philosopher_fn, &phil[i]);
	}
	
	pthread_exit(0);
	return 0;
}
