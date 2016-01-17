extern crate rand;

pub mod ga;
pub mod network;
pub mod xor_network;

use ga::*;
use xor_network::*;

fn main() {
    let mut experiment = XORNetworkExperiment::new();
    let mut ga = ga::GA::new();
    let mut population = ga.new_population(1000, &mut experiment);
    for _ in 1..200 {
        ga.evaluate_population(&mut population, &mut experiment);
        {
            let member = population.get_fittest_member();
            println!("{}", member.get_fitness());
        }
        population = ga.new_generation(&mut population, &mut experiment);
    }
    println!("Finished!");
}