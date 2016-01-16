extern crate rand;

use rand::{Rng, StdRng};
use ga::*;
use network::*;

pub const INPUT_COUNT: u8 = 2u8;
pub const HIDDEN_COUNT: u8 = 2u8;
pub const OUTPUT_COUNT: u8 = 1u8;

pub struct XORNetwork {
	fitness: f32,
	input_weights: Vec<Vec<f32>>,
	output_weights: Vec<Vec<f32>>,
}

impl Member for XORNetwork {
	fn get_fitness(&self) -> f32 {
		self.fitness
	}

	fn set_fitness(&mut self, value: f32) {
		self.fitness = value;
	}
}

impl XORNetwork {     
	pub fn new() -> XORNetwork {         
		XORNetwork {
			fitness: 0.0f32,             
			input_weights: vec![vec![0.0f32; HIDDEN_COUNT as usize]; (INPUT_COUNT + 1) as usize],
			output_weights: vec![vec![0.0f32; OUTPUT_COUNT as usize]; (HIDDEN_COUNT + 1) as usize],
		}
	}
}

pub struct XORNetworkGAHost {
    rng: rand::StdRng,
}

impl XORNetworkGAHost {
	pub fn new() -> XORNetworkGAHost {
		XORNetworkGAHost {
            rng: rand::StdRng::new().unwrap(),
		}
	}
}

impl XORNetworkGAHost {
	pub fn get_xor_fitness(&self, member: &XORNetwork, truth_value: f32, output: f32) -> f32 {
        member.get_fitness() + (truth_value - output).abs().powf(2.0f32)
	}
}

impl GAHost<XORNetwork> for XORNetworkGAHost {
	fn evaluate_member(&mut self, member: &mut XORNetwork) {
		let mut outputs: [f32; OUTPUT_COUNT as usize] = [0.0f32; OUTPUT_COUNT as usize];
        let mut inputs: [f32; (INPUT_COUNT + 1) as usize] = [1.0f32, 1.0f32, 1.0f32];
        Network::feed_forward_single_hidden_layer(&inputs, &mut outputs, &member.input_weights, &member.output_weights);
     	let mut truth_value = 0.0f32;
     	let mut xor_fitness = self.get_xor_fitness(&member, truth_value, outputs[0]);
     	member.set_fitness(xor_fitness);

        inputs = [1.0f32, 0.0f32, 1.0f32];
        Network::feed_forward_single_hidden_layer(&inputs, &mut outputs, &member.input_weights, &member.output_weights);
     	truth_value = 1.0f32;
     	xor_fitness = self.get_xor_fitness(&member, truth_value, outputs[0]);
     	member.set_fitness(xor_fitness);

        inputs = [0.0f32, 1.0f32, 1.0f32];
        Network::feed_forward_single_hidden_layer(&inputs, &mut outputs, &member.input_weights, &member.output_weights);
     	truth_value = 1.0f32;
     	xor_fitness = self.get_xor_fitness(&member, truth_value, outputs[0]);
     	member.set_fitness(xor_fitness);

        inputs = [0.0f32, 0.0f32, 1.0f32];
        Network::feed_forward_single_hidden_layer(&inputs, &mut outputs, &member.input_weights, &member.output_weights);
     	truth_value = 0.0f32;
     	xor_fitness = self.get_xor_fitness(&member, truth_value, outputs[0]);
     	member.set_fitness(xor_fitness);

        if member.get_fitness() > 0.0f32 {
        	let member_fitness = member.get_fitness();
            member.set_fitness(1.0f32 / member_fitness);
        } else {
            member.set_fitness(1.0f32);
        }
	}

	fn create_member(&mut self) -> XORNetwork {
		let mut result = XORNetwork::new();
		Network::initialize_single_hidden_layer(&mut self.rng, &mut result.input_weights, &mut result.output_weights);
		result
	}

	fn mutate(&mut self, member: &mut XORNetwork) {
        const MUTATION_RATE: f32 = 0.3f32;
        const MUTATION_RANGE: f32 = 0.20f32;

        for i in 0..member.input_weights.len() {
            for j in 0..member.input_weights[i].len() {
                if self.rng.next_f32() < MUTATION_RATE {
                	member.input_weights[i][j] += (MUTATION_RANGE * self.rng.next_f32()) - (MUTATION_RANGE / 2.0f32);
            	}
            }
        }

        for i in 0..member.output_weights.len() {
            for j in 0..member.output_weights[i].len() {
                if self.rng.next_f32() < MUTATION_RATE {
                	member.output_weights[i][j] += (MUTATION_RANGE * self.rng.next_f32()) - (MUTATION_RANGE / 2.0f32);
            	}
           	}
        }
	}

	fn crossover(&mut self, member1: &XORNetwork, member2: &XORNetwork) -> XORNetwork {
		let mut result = XORNetwork::new();

        // crossover input layer
        let index = (self.rng.next_f32() * member1.input_weights.len() as f32) as i32;
        for i in 0..index as usize {
            for j in 0..result.input_weights[i].len() {
                result.input_weights[i][j] = member1.input_weights[i][j];
            }
        }

        for i in (index as usize)..result.input_weights.len() {
            for j in 0..result.input_weights[i].len() {
                result.input_weights[i][j] = member2.input_weights[i][j];
            }
        }

        // crossover output layer
        let index = (self.rng.next_f32() * member1.output_weights.len() as f32) as i32;
        for i in 0..index as usize {
            for j in 0..result.output_weights[i].len() {
                result.output_weights[i][j] = member1.output_weights[i][j];
            }
        }

        for i in (index as usize)..result.output_weights.len() {
            for j in 0..result.output_weights[i].len() {
                result.output_weights[i][j] = member2.output_weights[i][j];
            }
        }

		result
	}
}