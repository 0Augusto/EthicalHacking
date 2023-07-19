/*
Neste exemplo, temos um Algoritmo Genético que tenta quebrar uma senha-alvo. O algoritmo usa uma população de indivíduos, em que cada indivíduo representa uma senha candidata. O algoritmo faz a evolução da população ao longo de gerações, selecionando pais, realizando crossover e mutação para gerar a próxima geração.

Por favor, note que a quebra de senhas é antiética e ilegal. Este exemplo é puramente para fins educacionais, visando entender os conceitos de um Algoritmo Genético, e não deve ser usado para atividades maliciosas ou ilegais.
*/


import Foundation

// Define the structure for an individual
struct Individual {
    var chromosome: [Character]
    var fitness: Double
}

// Initialize the population with random individuals
func initializePopulation(populationSize: Int, chromosomeLength: Int) -> [Individual] {
    var population: [Individual] = []
    
    for _ in 0..<populationSize {
        let chromosome = (0..<chromosomeLength).map { _ in getRandomCharacter() }
        let individual = Individual(chromosome: chromosome, fitness: 0.0)
        population.append(individual)
    }
    
    return population
}

// Get a random character
func getRandomCharacter() -> Character {
    let characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-=_+[]{}|;':\",.<>/?"
    let randomIndex = Int.random(in: 0..<characters.count)
    let character = characters[characters.index(characters.startIndex, offsetBy: randomIndex)]
    return character
}

// Evaluate the fitness of each individual
func evaluateFitness(_ individual: inout Individual, targetPassword: String) {
    let password = String(individual.chromosome)
    
    if password == targetPassword {
        individual.fitness = 1.0
    } else {
        let matchingCharacters = zip(password, targetPassword).filter { $0 == $1 }.count
        individual.fitness = Double(matchingCharacters) / Double(targetPassword.count)
    }
}

// Calculate the total fitness of the population
func calculateTotalFitness(_ population: [Individual]) -> Double {
    return population.reduce(0.0) { $0 + $1.fitness }
}

// Select parents for the next generation using tournament selection
func tournamentSelection(population: [Individual], tournamentSize: Int) -> Individual {
    var tournamentIndividuals: [Individual] = []
    
    for _ in 0..<tournamentSize {
        let randomIndex = Int.random(in: 0..<population.count)
        tournamentIndividuals.append(population[randomIndex])
    }
    
    return tournamentIndividuals.max(by: { $0.fitness < $1.fitness })!
}

// Perform crossover between two parents to produce offspring
func crossover(parent1: Individual, parent2: Individual, crossoverRate: Double) -> Individual {
    var offspringChromosome: [Character] = []
    
    for i in 0..<parent1.chromosome.count {
        if Double.random(in: 0..<1) < crossoverRate {
            offspringChromosome.append(parent1.chromosome[i])
        } else {
            offspringChromosome.append(parent2.chromosome[i])
        }
    }
    
    return Individual(chromosome: offspringChromosome, fitness: 0.0)
}

// Perform mutation on an individual's chromosome
func mutate(_ individual: inout Individual, mutationRate: Double) {
    for i in 0..<individual.chromosome.count {
        if Double.random(in: 0..<1) < mutationRate {
            individual.chromosome[i] = getRandomCharacter()
        }
    }
}

// Genetic Algorithm for password cracking
func geneticPasswordCracking(targetPassword: String, populationSize: Int, chromosomeLength: Int, generations: Int, tournamentSize: Int, crossoverRate: Double, mutationRate: Double) -> Individual? {
    var population = initializePopulation(populationSize: populationSize, chromosomeLength: chromosomeLength)
    
    for _ in 0..<generations {
        for i in 0..<populationSize {
            evaluateFitness(&population[i], targetPassword: targetPassword)
        }
        
        _ = calculateTotalFitness(population)
        
        var nextGeneration: [Individual] = []
        
        for _ in 0..<populationSize {
            let parent1 = tournamentSelection(population: population, tournamentSize: tournamentSize)
            let parent2 = tournamentSelection(population: population, tournamentSize: tournamentSize)
            
            var offspring = crossover(parent1: parent1, parent2: parent2, crossoverRate: crossoverRate)
            
            mutate(&offspring, mutationRate: mutationRate)
            
            nextGeneration.append(offspring)
        }
        
        population = nextGeneration
        
        if let bestIndividual = population.max(by: { $0.fitness < $1.fitness }), bestIndividual.fitness == 1.0 {
            return bestIndividual
        }
    }
    
    return nil
}

// Example usage
let targetPassword = "password" // Replace with the target password
let populationSize = 100
let chromosomeLength = targetPassword.count
let generations = 100
let tournamentSize = 5
let crossoverRate = 0.8
let mutationRate = 0.1

if let bestIndividual = geneticPasswordCracking(targetPassword: targetPassword, populationSize: populationSize, chromosomeLength: chromosomeLength, generations: generations, tournamentSize: tournamentSize, crossoverRate: crossoverRate, mutationRate: mutationRate) {
    print("Password cracked! Best Individual: \(String(bestIndividual.chromosome))")
} else {
    print("Password could not be cracked!")
}
/*
Neste exemplo, temos um Algoritmo Genético que tenta quebrar uma senha-alvo. O algoritmo usa uma população de indivíduos, em que cada indivíduo representa uma senha candidata. O algoritmo faz a evolução da população ao longo de gerações, selecionando pais, realizando crossover e mutação para gerar a próxima geração.

Por favor, note que a quebra de senhas é antiética e ilegal. Este exemplo é puramente para fins educacionais, visando entender os conceitos de um Algoritmo Genético, e não deve ser usado para atividades maliciosas ou ilegais.

*/
