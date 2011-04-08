/*
    Copyright (C) 2009 Luca Tonin <luca.tonin@epfl.ch>
    Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libcnbiprotocol library

    The libndf library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "generator.h"
#include <sys/time.h>

void co_generator_print(float* probs_array, int num_classes) {
	/*
	 * Print the probability array
	 */
	int i;
	float sum = 0;

	for (i = 0; i < num_classes; i++) {
		sum = sum + probs_array[i];
		printf("%f ",probs_array[i]);
	}
	printf(" Sum: %f\n", sum);
}

void co_generator_create (float* probs_array, int num_classes, int selected_class) {
	float target_prob;
	float curr_selected;
	float limit;
	float max_prob;
	int i, idx_max;	
	float sum_rest = 0;

	/*
	 * Generating target probabilities between 1/NumOfClasses and 1.0
	 */
	target_prob = co_generator_rand((float)1/num_classes, 1.0);
	
	/*
	 * Save probability in the first array position
	 */
	probs_array[0] = target_prob;
	
	/*
	 * Generating the other probabilities
	 * */

	limit = 1.0;
	for(i = 1; i < num_classes -1; i++) {
			
		limit = limit - probs_array[i-1];
		probs_array[i] = co_generator_rand(0.0, limit);
		sum_rest = sum_rest + probs_array[i];
	}
	
	probs_array[num_classes-1] = 1 - (sum_rest + probs_array[0]);

	/*
	 * Check where is the max. With the algoritm above if the target
	 * probability is low could happen that an other prob non
	 * target is bigger.
	 */
	max_prob = 0.0;
	idx_max = -1;
	for (i =0; i< num_classes; i++) {
		if (probs_array[i]>max_prob) {
			max_prob = probs_array[i];
			idx_max = i;
		}
	}

	/*
	 * Reshape the array according to the selected class
	 */
	if (selected_class < 0 || selected_class > num_classes) {
		fprintf(stderr, "The selected class doesn't exist");
		return;
	} else {
		curr_selected = probs_array[selected_class];
		probs_array[selected_class] = probs_array[idx_max];
		probs_array[idx_max] = curr_selected;
	}
}

float co_generator_rand(float min, float max) {
	/*
	 * Random float generation between min and max values
	 */
	float rand_value;
	struct timeval time;
	gettimeofday(&time, NULL);
	srand((time.tv_usec));
	rand_value = min + (max-min)*rand()/((float)RAND_MAX);
	return rand_value;
};
