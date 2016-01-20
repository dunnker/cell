extern crate rand;
use rand::{ Rng, StdRng };

pub trait Member {
    fn get_fitness(&self) -> f32;
    fn set_fitness(&mut self, value: f32);
}

pub struct Population<M: Member> {
    members: Vec<M>,
}

/// Keeps a vector of Member's
impl<M> Population<M> where M: Member {
    pub fn new() -> Population<M> {
        Population {
            members: Vec::new(),
        }
    }

    pub fn get_members(&mut self) -> &mut Vec<M> {
        &mut self.members
    }

    pub fn add_member(&mut self, member: M)  {
        self.members.push(member);
    }

    pub fn get_fittest_member(&self) -> &M {
        assert!(self.members.len() > 1);

        let mut result: Option<&M> = None;
        for i in 0..self.members.len() {
            let better_fitness =
                match result {
                    None => true,
                    Some(ref p) => self.members[i].get_fitness() > p.get_fitness()
                };
            if better_fitness {
                result = Some(&self.members[i]);
            }
        }
        match result {
            None => &self.members[0], // should never happen per assert above
            Some(ref p) => &p
        }
    }

    /// Use tournament selection to find a random member in the population
    pub fn get_random_member(&self, tournament_size: u16, rng: &mut rand::StdRng) -> &M {
        assert!(self.members.len() > 1);

        // choose member with best fitness out of the tournament's random members
        let mut result: Option<&M> = None;
        for i in 0..tournament_size {
            let index: usize = rng.gen_range(0, self.members.len());
            let better_fitness =
                match result {
                    None => true,
                    Some(ref p) => self.members[index].get_fitness() > p.get_fitness()
                };
            if better_fitness {
                result = Some(&self.members[index]);
            }
        }

        match result {
            None => &self.members[0], // should never happen per assert above
            Some(ref p) => &p
        }
    }
}

pub trait Experiment<M: Member> {
    fn create_member(&mut self) -> M;
    fn evaluate_member(&mut self, member: &mut M);
    fn mutate(&mut self, member: &mut M);
    fn crossover(&mut self, member1: &M, member2: &M) -> M;
}

pub struct GA {
    rng: rand::StdRng,
}

impl GA {
    pub fn new() -> GA {
        GA {
            rng: rand::StdRng::new().unwrap(),
        }
    }

    pub fn new_population<M: Member>(&self, member_count: u16, experiment: &mut Experiment<M>) -> Population<M> {
        let mut result: Population<M> = Population::new();
        for _ in 0..member_count {
            let member = experiment.create_member();
            result.add_member(member);
        }
        result
    }

    pub fn evaluate_population<M: Member>(&self, population: &mut Population<M>, experiment: &mut Experiment<M>) {
        for mut member in population.get_members() {
            experiment.evaluate_member(&mut member);
        }
    }

    pub fn new_generation<M: Member>(&mut self, population: &mut Population<M>, experiment: &mut Experiment<M>) -> Population<M> {
        // tournament size is 5% of the total population size
        let mut tournament_size: u16 = ((population.get_members().len() as f32) * 0.04f32) as u16;
        if tournament_size < 2 {
            tournament_size = 2;
        }

        let mut result: Population<M> = Population::new();
        for _ in 0..population.get_members().len() {
            let member1 = population.get_random_member(tournament_size, &mut self.rng);
            let member2 = population.get_random_member(tournament_size, &mut self.rng);

            let mut new_member = experiment.crossover(member1, member2);

            experiment.mutate(&mut new_member);

            result.add_member(new_member);
        }

        result
    }
}