#include "pch.h"
#include "CppUnitTest.h"

#include "City.h"
#include "CrossoverOperators.h"
#include "MutationOperators.h"
#include "Population.h"
#include "RandomUtil.h"
#include "SelectionOperators.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace TSPSolverTests {
	vector<City> initCities(int dimension) {
		vector<City> cities;
		for (int i = 0; i < dimension; i++) {
			cities.push_back(City(i + 1, RandomUtil::getRandomInt(-10, 10), RandomUtil::getRandomInt(-10, 10)));
		}

		RandomUtil::shuffleVector(cities, 0, cities.size() - 1);

		return cities;
	}

	Individual initIndividual(int dimension) {
		vector<City> cities = initCities(dimension);

		Individual ind = Individual(cities);

		return ind;
	}

	Population initPopulation(int populationSize, int dimension) {
		vector<City> cities = initCities(dimension);

		Population population = Population(cities, populationSize);

		return population;
	}

	TEST_CLASS(MutationTests) {
	public:
		void testMutation(MutationOperator* mutator, Individual& individual) {
			for (int test = 0; test < 1000; test++) {
				unordered_set<int> indexSet;
				for (int i = 0; i < individual.dimension; i++) {
					indexSet.insert(individual.route[i].index);
				}
				mutator->mutate(individual);

				for (City& city : individual.route) {
					if (indexSet.find(city.index) != indexSet.end()) {
						indexSet.erase(city.index);
					} else {
						Assert::Fail();
					}
				}

				Assert::IsTrue(indexSet.size() == 0);
			}
		}
		TEST_METHOD(InsertMutationTest) {
			MutationOperator* mutator = new InsertMutation();
			Individual ind = initIndividual(50);

			testMutation(mutator, ind);

			delete mutator;
		}

		TEST_METHOD(SwapMutationTest) {
			MutationOperator* mutator = new SwapMutation();
			Individual ind = initIndividual(50);

			testMutation(mutator, ind);

			delete mutator;
		}

		TEST_METHOD(InversionMutationTest) {
			MutationOperator* mutator = new InversionMutation();
			Individual ind = initIndividual(50);

			testMutation(mutator, ind);

			delete mutator;
		}

		TEST_METHOD(ScrambleMutationTest) {
			MutationOperator* mutator = new ScrambleMutation();
			Individual ind = initIndividual(50);

			testMutation(mutator, ind);

			delete mutator;
		}
	};

	TEST_CLASS(CrossoverTests) {
	public:
		void testCrossover(CrossoverOperator* crossover, Individual& ind1, Individual& ind2) {
			for (int test = 0; test < 1000; test++) {
				unordered_set<int> indexSet;
				for (int i = 0; i < ind1.dimension; i++) {
					indexSet.insert(ind1.route[i].index);
				}
				
				Individual child = crossover->crossover(ind1, ind2);
				string childRoute = child.toString();
				wstring wideString(childRoute.begin(), childRoute.end());
				for (City& city : child.route) {
					if (indexSet.find(city.index) != indexSet.end()) {
						indexSet.erase(city.index);
					} else {
						Assert::Fail(wideString.c_str());
					}
				}

				Assert::IsTrue(indexSet.size() == 0, wideString.c_str());
			}
		}
		TEST_METHOD(OrderCrossoverTest) {
			int dimension = 50;
			CrossoverOperator* crossover = new OrderCrossover(dimension);
			Individual ind1 = initIndividual(dimension);
			Individual ind2 = initIndividual(dimension);

			testCrossover(crossover, ind1, ind2);

			delete crossover;
		}

		TEST_METHOD(PMXCrossoverTest) {
			int dimension = 50;
			CrossoverOperator* crossover = new PMXCrossover(dimension);
			Individual ind1 = initIndividual(dimension);
			Individual ind2 = initIndividual(dimension);

			testCrossover(crossover, ind1, ind2);

			delete crossover;
		}

		TEST_METHOD(CycleCrossoverTest) {
			int dimension = 50;
			CrossoverOperator* crossover = new CycleCrossover(dimension);
			Individual ind1 = initIndividual(dimension);
			Individual ind2 = initIndividual(dimension);

			testCrossover(crossover, ind1, ind2);

			delete crossover;
		}
	};

	TEST_CLASS(SelectionTests) {
	public:
		void runSelectionTest(SelectionOperator* selector, Population& pop, Individual inds[]) {
			for (int i = 0; i < 1000; i++) {
				selector->select(pop, inds);
			}
		}
		TEST_METHOD(FitnessProportionalTest) {
			int populationSize = 50;
			int dimension = 50;
			Population population = initPopulation(populationSize, dimension);
			population.calculateAllFitness();
			Individual* selectedIndividuals = new Individual[populationSize];

			SelectionOperator* selector = new FitnessProportionalSelection(populationSize);

			runSelectionTest(selector, population, selectedIndividuals);

			delete selector;
			delete[] selectedIndividuals;
		}

		TEST_METHOD(TournamentTest) {
			int populationSize = 50;
			int dimension = 50;
			int tournamentSize = 5;

			Population population = initPopulation(populationSize, dimension);
			population.calculateAllFitness();
			Individual* selectedIndividuals = new Individual[populationSize];

			SelectionOperator* selector = new TournamentSelection(tournamentSize);

			runSelectionTest(selector, population, selectedIndividuals);

			delete selector;
			delete[] selectedIndividuals;
		}

		TEST_METHOD(ElitismTest) {
			int populationSize = 50;
			int dimension = 50;
			Population population = initPopulation(populationSize, dimension);
			population.calculateAllFitness();
			Individual* selectedIndividuals = new Individual[populationSize];

			SelectionOperator* selector = new ElitismSelection();

			runSelectionTest(selector, population, selectedIndividuals);

			delete selector;
			delete[] selectedIndividuals;
		}
	};
}
