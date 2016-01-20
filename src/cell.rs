extern crate rand;

use rand::{Rng, StdRng};
use ga::*;
use network::*;

#[derive(Copy, Clone, Default)]
pub struct Cell {
	pub alive: bool,
	pub generation: u32,
}

pub const WORLD_SIZE: usize = 10;

pub struct CellMember {
    fitness: f32,
    cells: [[Cell; WORLD_SIZE]; WORLD_SIZE],
    input_weights: Vec<Vec<f32>>,
    output_weights: Vec<Vec<f32>>,	
}

impl Member for CellMember {
    fn get_fitness(&self) -> f32 {
        self.fitness
    }

    fn set_fitness(&mut self, value: f32) {
        self.fitness = value;
    }
}

pub const INPUT_COUNT: usize = 9;
pub const HIDDEN_COUNT: usize = 9;
pub const OUTPUT_COUNT: usize = 4;

impl CellMember {     
    pub fn new() -> CellMember {         
        let mut cell_member = CellMember {
            fitness: 0.0f32,      
            cells: [[Cell::default(); WORLD_SIZE]; WORLD_SIZE],       
            input_weights: vec![vec![0.0f32; HIDDEN_COUNT]; INPUT_COUNT + 1],
            output_weights: vec![vec![0.0f32; OUTPUT_COUNT]; HIDDEN_COUNT + 1],
        };
        cell_member.cells[0][0].alive = true;
        cell_member
    }

    pub fn get_cells(&self) -> [[Cell; WORLD_SIZE]; WORLD_SIZE] {
        self.cells
    }
}

pub struct CellExperiment {
    rng: rand::StdRng,
    model: [[bool; WORLD_SIZE]; WORLD_SIZE],
}

impl CellExperiment {
    pub fn new() -> CellExperiment {
        CellExperiment {
            rng: rand::StdRng::new().unwrap(),
            model: [[false; WORLD_SIZE]; WORLD_SIZE],
        }
    }

    pub fn get_model(&mut self) -> &mut [[bool; WORLD_SIZE]; WORLD_SIZE] {
        &mut self.model
    }

    fn assign_position_inputs(&self, member: &CellMember, inputs: &mut [f32; INPUT_COUNT + 1], index: usize, 
        x: i32, y: i32)
    {
        if x >= 0 && (x as usize) < WORLD_SIZE && y >= 0 && (y as usize) < WORLD_SIZE && 
            member.cells[x as usize][y as usize].alive {
            inputs[index] = 1.0f32;
        } else {
            inputs[index] = 0.0f32;
        }
    }

    fn evaluate_cell(&mut self, inputs: &mut [f32; INPUT_COUNT + 1], outputs: &mut [f32; OUTPUT_COUNT], 
        member: &mut CellMember, x: i32, y: i32) {
        if member.cells[x as usize][y as usize].alive {
            let inputs_len = inputs.len();
            inputs[inputs_len - 1] = 1.0f32; // bias

            self.assign_position_inputs(member, inputs, 0, x - 1, y - 1);
            self.assign_position_inputs(member, inputs, 1, x, y - 1);
            self.assign_position_inputs(member, inputs, 2, x + 1, y - 1);
            self.assign_position_inputs(member, inputs, 3, x - 1, y);
            self.assign_position_inputs(member, inputs, 4, x + 1, y);
            self.assign_position_inputs(member, inputs, 5, x - 1, y + 1);
            self.assign_position_inputs(member, inputs, 6, x, y + 1);
            self.assign_position_inputs(member, inputs, 7, x + 1, y + 1);

            inputs[8] = member.cells[x as usize][y as usize].generation as f32;

            Network::feed_forward_single_hidden_layer(inputs, outputs, &member.input_weights, &member.output_weights);

            if outputs[0] > 0.5 || outputs[0] < -0.5 {
                let mut next_x: i32 = 0;
                let mut next_y: i32 = 0;
                if outputs[1] > 0.5 {
                    next_x = 1;
                } else if outputs[1] < -0.5 {
                    next_x = -1;
                }
                if outputs[2] > 0.5 {
                    next_y = 1;
                } else if outputs[2] < -0.5 {
                    next_y = -1;
                }
                if next_x != 0 || next_y != 0 {
                    next_x += x;
                    next_y += y;
                    if next_x >= 0 && (next_x as usize) < WORLD_SIZE && next_y >= 0 && (next_y as usize) < WORLD_SIZE && 
                        !member.cells[next_x as usize][next_y as usize].alive {
                        member.cells[next_x as usize][next_y as usize].alive = true;
                        member.cells[next_x as usize][next_y as usize].generation = 
                            member.cells[x as usize][y as usize].generation + 1;
                    }
                }
            }

            if outputs[3] < -0.5 {
                member.cells[x as usize][y as usize].alive = false;
            } else if outputs[3] > 0.5 {
                member.cells[x as usize][y as usize].alive = false;
            }
        }
    }
}

impl Experiment<CellMember> for CellExperiment {
    fn evaluate_member(&mut self, member: &mut CellMember) {
        let mut inputs: [f32; INPUT_COUNT + 1] = [0.0f32; INPUT_COUNT + 1];
        let mut outputs: [f32; OUTPUT_COUNT] = [0.0f32; OUTPUT_COUNT];

        let turn_count = WORLD_SIZE * WORLD_SIZE;

        for _ in 0..turn_count {
            for x in 0..WORLD_SIZE {
                for y in 0..WORLD_SIZE {
                    self.evaluate_cell(&mut inputs, &mut outputs, member, x as i32, y as i32);
                }
            }
        }

        for x in 0..WORLD_SIZE {
            for y in 0..WORLD_SIZE {
                if self.model[x][y] {
                    if member.cells[x][y].alive {
                        member.fitness += 10f32;
                    } else {
                        member.fitness -= 1f32;
                    }
                } else {
                    if member.cells[x][y].alive {
                        member.fitness -= 5f32;
                    } else {
                        member.fitness += 0.5f32;
                    }
                }
            }
        }
    }

    fn create_member(&mut self) -> CellMember {
        let mut result = CellMember::new();
        Network::initialize_single_hidden_layer(&mut self.rng, &mut result.input_weights, &mut result.output_weights);
        result
    }

    fn mutate(&mut self, member: &mut CellMember) {
        const MUTATION_RATE: f32 = 0.3f32;
        const MUTATION_RANGE: f32 = 5.0f32;

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

    fn crossover(&mut self, member1: &CellMember, member2: &CellMember) -> CellMember {
        let mut result = CellMember::new();

        // crossover input layer
        let index = self.rng.gen_range(0, member1.input_weights.len()) as i32;
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
        let index = self.rng.gen_range(0, member1.output_weights.len()) as i32;
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