import numpy as np
import random
from typing import List, Tuple, Optional


class GeneticAlgorithm:

    def __init__(
            self,
            pop_size: int,  # Population size
            generations: int,  # Number of generations for the algorithm
            mutation_rate: float,  # Gene mutation rate
            crossover_rate: float,  # Gene crossover rate
            tournament_size: int,  # Tournament size for selection
            elitism: bool,  # Whether to apply elitism strategy
            random_seed: Optional[int],  # Random seed for reproducibility
    ):
        # Students need to set the algorithm parameters
        self.pop_size = pop_size
        self.generations = generations
        self.mutation_rate = mutation_rate
        self.crossover_rate = crossover_rate
        self.tournament_size = tournament_size
        self.elitism = elitism

        if random_seed is not None:
            random.seed(random_seed)
            np.random.seed(random_seed)

    def _ensure_all_students_assigned(self, individual: List[int],
                                      M: int) -> List[int]:
        """
        Ensure that every student is assigned at least one task.
        :param individual: Individual
        :param M: Number of students
        :return: Individual with at least one task assigned to each student
        """
        assigned_student = set(individual)
        unassigned_student = set(range(1, M + 1)) - assigned_student

        cnt_table = {s: individual.count(s) for s in assigned_student}
        for student in unassigned_student:
            # Find students with multiple assignments
            multiple_assignments = [
                s for s, count in cnt_table.items() if count > 1
            ]
            if multiple_assignments:
                # Choose a random student with multiple assignments
                student_to_replace = random.choice(multiple_assignments)
                # Find positions where this student is assigned
                positions = [
                    i for i, s in enumerate(individual)
                    if s == student_to_replace
                ]
                # Replace one of these positions with the unassigned student
                replace_pos = random.choice(positions)
                individual[replace_pos] = student
                # Update the counter
                cnt_table[student_to_replace] -= 1
                cnt_table[student] = 1

        return individual

    def _init_population(self, M: int, N: int) -> List[List[int]]:
        """
        Initialize the population and generate random individuals, ensuring that every student is assigned at least one task.
        :param M: Number of students
        :param N: Number of tasks
        :return: Initialized population
        """
        random_population = []
        for i in range(self.pop_size):
            # Arrange tasks for each student first and then randomly assign the remaining tasks
            # Then shuffle to get a random arrangement
            arrangement = list(range(
                1, M + 1)) + [random.randint(1, M) for _ in range(N - M)]
            random.shuffle(arrangement)

            random_population.append(arrangement)
        return random_population

    def _fitness(self, individual: List[int],
                 student_times: np.ndarray) -> float:
        """
        Fitness function: calculate the fitness value of an individual.
        :param individual: Individual
        :param student_times: Time required for each student to complete each task
        :return: Fitness value
        """
        time = 0
        for task, student in enumerate(individual):
            time += student_times[student - 1][task]
        return time

    def _selection(self, population: List[List[int]],
                   fitness_scores: List[float]) -> List[int]:
        """
        Use tournament selection to choose parents for crossover.
        :param population: Current population
        :param fitness_scores: Fitness scores for each individual
        :return: Selected parent
        """
        if self.elitism:
            # Elitism strategy: select the best individual directly
            best_idx = fitness_scores.index(min(fitness_scores))
            return population[best_idx]
        else:
            # Tournament selection: randomly select a few individuals and choose the best one
            selected_idx = []
            for i in range(self.tournament_size):
                idx = random.randint(0, len(population) - 1)
                while (idx in selected_idx):
                    idx = random.randint(0, len(population) - 1)

                selected_idx.append(idx)
            sorted_idx = sorted(selected_idx, key=lambda x: fitness_scores[x])
            return population[sorted_idx[0]]

    def _crossover(self, parent1: List[int], parent2: List[int],
                   M: int) -> Tuple[List[int], List[int]]:
        """
        Crossover: generate two offspring from two parents.
        :param parent1: Parent 1
        :param parent2: Parent 2
        :param M: Number of students
        :return: Generated offspring
        """
        # Single-point crossover
        # [:0] will return an empty list, so we need to add 1 to the index
        if random.random() < self.crossover_rate:
            crossover_point = random.randint(1, len(parent1) - 1)
            offspring1 = parent1[:crossover_point] + parent2[crossover_point:]
            offspring2 = parent2[:crossover_point] + parent1[crossover_point:]

            # Ensure all students are assigned in both offspring
            offspring1 = self._ensure_all_students_assigned(offspring1, M)
            offspring2 = self._ensure_all_students_assigned(offspring2, M)
        else:
            offspring1 = parent1
            offspring2 = parent2
        return (offspring1, offspring2)

    def _mutate(self, individual: List[int], M: int) -> List[int]:
        """
        Mutation operation: randomly change some genes (task assignments) of the individual.
        :param individual: Individual
        :param M: Number of students
        :return: Mutated individual
        """
        cnt_table = {s: individual.count(s) for s in set(individual)}
        for i in range(len(individual)):
            if random.random() < self.mutation_rate and cnt_table[
                    individual[i]] > 1:
                individual[i] = random.randint(1, M)

    def __call__(self, M: int, N: int,
                 student_times: np.ndarray) -> Tuple[List[int], int]:
        """
        Execute the genetic algorithm and return the optimal solution (allocation plan) and its total time cost.
        :param M: Number of students
        :param N: Number of tasks
        :param student_times: Time required for each student to complete each task
        :return: Optimal allocation plan and total time cost
        """
        # Initialize the population
        population = self._init_population(M, N)
        for generation in range(self.generations):
            # Calculate the fitness value for each individual
            fitness_scores = [
                self._fitness(individual, student_times)
                for individual in population
            ]

            # Selection
            new_population = []
            for _ in range(self.pop_size):
                parent1 = self._selection(population, fitness_scores)
                parent2 = self._selection(population, fitness_scores)

                # Crossover
                offspring1, offspring2 = self._crossover(parent1, parent2, M)

                # Mutation
                self._mutate(offspring1, M)
                self._mutate(offspring2, M)

                new_population.append(offspring1)
                new_population.append(offspring2)

            population = new_population
        best = sorted(population,
                      key=lambda x: self._fitness(x, student_times))[0]
        return best, self._fitness(best, student_times)


if __name__ == "__main__":

    def write_output_to_file(problem_num: int,
                             total_time: int,
                             filename: str = "answer.txt") -> None:
        """
        Write results to a file and check if the format is correct.
        """
        print(f"Problem {problem_num}: Total time = {total_time}")

        if not isinstance(total_time, int) and not isinstance(
                total_time, np.int32):
            raise ValueError(
                f"Invalid format for problem {problem_num}. Total time should be an integer."
            )

        with open(filename, 'a') as file:
            file.write(f"Total time = {total_time}\n")

    # TODO: Define multiple test problems based on the examples and solve them using the genetic algorithm
    # Example problem 1 (define multiple problems based on the given example format)
    # M, N = 2, 3
    # student_times = [[3, 8, 6],
    #                  [5, 2, 7]]

    M1, N1 = 2, 3
    cost1 = [[3, 2, 4], [4, 3, 2]]

    M2, N2 = 4, 4
    cost2 = [[5, 6, 7, 4], [4, 5, 6, 3], [6, 4, 5, 2], [3, 2, 4, 5]]

    M3, N3 = 8, 9
    cost3 = [[90, 100, 60, 5, 50, 1, 100, 80, 70],
             [100, 5, 90, 100, 50, 70, 60, 90, 100],
             [50, 1, 100, 70, 90, 60, 80, 100, 4],
             [60, 100, 1, 80, 70, 90, 100, 50, 100],
             [70, 90, 50, 100, 100, 4, 1, 60, 80],
             [100, 60, 100, 90, 80, 5, 70, 100, 50],
             [100, 4, 80, 100, 90, 70, 50, 1, 60],
             [1, 90, 100, 50, 60, 80, 100, 70, 5]]

    M4, N4 = 3, 3
    cost4 = [[2, 5, 6], [4, 3, 5], [5, 6, 2]]

    M5, N5 = 4, 4
    cost5 = [[4, 5, 1, 6], [9, 1, 2, 6], [6, 9, 3, 5], [2, 4, 5, 2]]

    M6, N6 = 4, 4
    cost6 = [[5, 4, 6, 7], [8, 3, 4, 6], [6, 7, 3, 8], [7, 8, 9, 2]]

    M7, N7 = 4, 4
    cost7 = [[4, 7, 8, 9], [6, 3, 6, 7], [8, 6, 2, 6], [7, 8, 7, 3]]

    M8, N8 = 5, 5
    cost8 = [[8, 8, 24, 24, 24], [6, 18, 6, 18, 18], [30, 10, 30, 10, 30],
             [21, 21, 21, 7, 7], [27, 27, 9, 27, 9]]

    M9, N9 = 5, 5
    cost9 = [[10, 10, 100, 100, 100], [12, 100, 100, 12, 12],
             [100, 15, 15, 100, 100], [11, 100, 11, 100, 100],
             [100, 14, 100, 14, 14]]

    M10, N10 = 9, 10
    cost10 = [[1, 90, 100, 50, 70, 20, 100, 60, 80, 90],
              [100, 10, 1, 100, 60, 80, 70, 100, 50, 90],
              [90, 50, 70, 1, 100, 100, 60, 90, 80, 100],
              [70, 100, 90, 5, 10, 60, 100, 80, 90, 50],
              [50, 100, 100, 90, 20, 4, 80, 70, 60, 100],
              [100, 5, 80, 70, 90, 100, 4, 50, 1, 60],
              [90, 60, 50, 4, 100, 90, 100, 5, 10, 80],
              [100, 70, 90, 100, 4, 60, 1, 90, 100, 5],
              [80, 100, 5, 60, 50, 90, 70, 100, 4, 1]]

    problems = [(M1, N1, np.array(cost1)), (M2, N2, np.array(cost2)),
                (M3, N3, np.array(cost3)), (M4, N4, np.array(cost4)),
                (M5, N5, np.array(cost5)), (M6, N6, np.array(cost6)),
                (M7, N7, np.array(cost7)), (M8, N8, np.array(cost8)),
                (M9, N9, np.array(cost9)), (M10, N10, np.array(cost10))]

    # Example for GA execution:
    # TODO: Please set the parameters for the genetic algorithm
    ga = GeneticAlgorithm(pop_size=500,
                          generations=20,
                          mutation_rate=0.1,
                          crossover_rate=0.9,
                          tournament_size=10,
                          elitism=False,
                          random_seed=42)

    # Solve each problem and immediately write the results to the file
    for i, (M, N, student_times) in enumerate(problems, 1):
        best_allocation, total_time = ga(M=M, N=N, student_times=student_times)
        write_output_to_file(i, total_time)
        print(f"Problem {i}: Best allocation = {best_allocation}")

    print("Results have been written to results.txt")
