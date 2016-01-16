extern crate rand;
use rand::{Rng, StdRng};
use std;

pub struct SigmoidActivationFunction;

impl SigmoidActivationFunction {
	pub fn process_value(value: f32) -> f32 {
		let e = std::f32::consts::E;
		1.0f32 / (1.0f32 + e.powf(-value))
	}
}

pub struct Network;

impl Network {
	pub fn initialize_single_hidden_layer(rng: &mut rand::StdRng, input_weights: &mut Vec<Vec<f32>>, 
		output_weights: &mut Vec<Vec<f32>>) {
		// assign random weight values
		for n in 0..input_weights.len() {
			for w in 0..input_weights[n].len() {
				input_weights[n][w] = (rng.next_f32() * 4.0f32) - 2.0f32;
			}
		}
		for n in 0..output_weights.len() {
			for w in 0..output_weights[n].len() {
				output_weights[n][w] = (rng.next_f32() * 4.0f32) - 2.0f32;
			}
		}
	}

	pub fn feed_forward_single_hidden_layer(inputs: &[f32], outputs: &mut [f32], input_weights: &Vec<Vec<f32>>, 
		output_weights: &Vec<Vec<f32>>) {
		assert!(input_weights.len() > 0);
		let mut hidden: Vec<f32> = vec![0.0f32; input_weights[0].len() + 1];
		// set bias node
		let hidden_len = hidden.len();
		hidden[hidden_len - 1] = 1.0f32;

		// iterate over nodes of hidden layer, skipping the last bias node
		for i in 0..hidden.len() - 1 {
			let mut sum = 0.0f32;
			for j in 0..inputs.len() {
				sum += inputs[j] * input_weights[j][i];
			}
			hidden[i] = SigmoidActivationFunction::process_value(sum);
		}

		for i in 0..outputs.len() {
			let mut sum = 0.0f32;
			for j in 0..hidden.len() {
				sum += hidden[j] * output_weights[j][i];
			}
			outputs[i] = SigmoidActivationFunction::process_value(sum);
		}
	}
}