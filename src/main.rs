extern crate piston;
extern crate graphics;
extern crate glutin_window;
extern crate opengl_graphics;
extern crate rand;

pub mod ga;
pub mod network;
pub mod cell;
//pub mod xor_network;

use piston::window::{ AdvancedWindow, WindowSettings };
use glutin_window::GlutinWindow as Window;
use piston::event_loop::*;
use opengl_graphics::{ GlGraphics, OpenGL };
use opengl_graphics::glyph_cache::GlyphCache;
use graphics::{ Transformed };
use piston::input::*;
use std::path::Path;
use std::fs::OpenOptions;

use ga::*;
use cell::*;

//const WHITE: [f32; 4] = [1.0, 1.0, 1.0, 1.0];
const BLACK: [f32; 4] = [0.0, 0.0, 0.0, 1.0];
const LIGHT_GRAY: [f32; 4] = [0.3, 0.3, 0.3, 1.0];
//const DARK_GRAY: [f32; 4] = [0.1, 0.1, 0.1, 1.0];
const GREEN: [f32; 4] = [0.0, 1.0, 0.0, 1.0];
//const CYAN: [f32; 4] = [0.0, 1.0, 1.0, 1.0];
const RED: [f32; 4] = [1.0, 0.0, 0.0, 1.0];
const BLUE: [f32; 4] = [0.0, 0.0, 1.0, 1.0];
const ORANGE: [f32; 4] = [1.0, 0.5, 0.0, 1.0];
//const YELLOW: [f32; 4] = [1.0, 1.0, 0.0, 1.0];
//const PURPLE: [f32; 4] = [0.5, 0.0, 1.0, 1.0];

const TEXT_FONT_SIZE: u32 = 22;
const CELL_SIZE: f64 = 30.0;
const LEFT_MARGIN: f64 = 20f64;
const TOP_MARGIN: f64 = 30f64;
const LINE_HEIGHT: f64 = 40f64;
const STATUS_LEFT_MARGIN: f64 = 400f64;
const STATUS_TOP_MARGIN: f64 = 100f64;

const POP_COUNT: u16 = 500;

struct App {
    gl: GlGraphics,
    cache: GlyphCache<'static>,
    ga: GA,
    generation: u32,
    experiment: CellExperiment,
    population: Option<Population<CellMember>>,
    member_fitness: f32,
    member_cells: [[Cell; WORLD_SIZE]; WORLD_SIZE],
}

struct Render;

impl Render {
}

impl App {
    fn render(&mut self, args: &RenderArgs) {
        let use_model = self.experiment.get_model();
        let use_cache = &mut self.cache;
        let use_generation = self.generation;
        let use_member_fitness = self.member_fitness;
        let use_member_cells = self.member_cells;

        self.gl.draw(args.viewport(), |c, gl| {
            // clear the viewport
            graphics::clear(BLACK, gl);

            let mut text = graphics::Text::new(TEXT_FONT_SIZE);
            text.color = ORANGE;
            let mut transform: graphics::context::Context = c.trans(STATUS_LEFT_MARGIN, STATUS_TOP_MARGIN);
            text.draw("Press 'S' to start", use_cache, &c.draw_state, 
                transform.transform, gl);
            transform = transform.trans(0f64, LINE_HEIGHT);

            text.draw(&format!("Generation: {}", use_generation), use_cache, &c.draw_state, 
                transform.transform, gl);
            transform = transform.trans(0f64, LINE_HEIGHT);

            text.draw(&format!("Best: {}", use_member_fitness), use_cache, &c.draw_state, 
                transform.transform, gl);
            transform = transform.trans(0f64, LINE_HEIGHT);

            let rect_border = graphics::Rectangle::new_border(LIGHT_GRAY, 1.5);
            rect_border.draw([
                LEFT_MARGIN - 2f64,
                TOP_MARGIN - 2f64,
                (CELL_SIZE * WORLD_SIZE as f64) + 3f64,
                (CELL_SIZE * WORLD_SIZE as f64) + 3f64,
            ], &c.draw_state, c.transform, gl);

            for x in 0..WORLD_SIZE {
                for y in 0..WORLD_SIZE {
                    let (cell_x, cell_y) = (x as f64 * CELL_SIZE, y as f64 * CELL_SIZE);
                    let transform = c.transform.trans(LEFT_MARGIN, TOP_MARGIN).trans(cell_x, cell_y);
                    let square = graphics::rectangle::square(0f64, 0f64, CELL_SIZE - 1f64);
                    if use_model[x][y] {
                        let mut rectangle = graphics::Rectangle::new(BLUE);
                        rectangle.shape = graphics::rectangle::Shape::Round(4.0, 16);
                        rectangle.draw(square, &c.draw_state, transform, gl);
                    }

                    if use_member_cells[x][y].alive {
                        let mut rectangle = 
                            if use_model[x][y] { 
                                graphics::Rectangle::new(GREEN)
                            } else {
                                graphics::Rectangle::new(RED)
                            };

                        rectangle.shape = graphics::rectangle::Shape::Round(4.0, 16);
                        rectangle.draw(square, &c.draw_state, transform, gl);
                    }
                }
            }
        });
    }

    fn update(&mut self, args: &UpdateArgs) {

    }

    fn key_pressed(&mut self, key: keyboard::Key) {
        match key {
            Key::S => {
                if self.population.is_none() {
                    self.generation = 0;
                    self.member_fitness = 0.0f32;
                    self.member_cells = [[Cell::default(); WORLD_SIZE]; WORLD_SIZE];
                    self.population = Some(self.ga.new_population(POP_COUNT, &mut self.experiment));
                }

                let mut new_population: Option<Population<CellMember>> = None;
                {
                    let mut some_population = self.population.as_mut().unwrap();
                    self.ga.evaluate_population(&mut some_population, &mut self.experiment);
                    {
                        let member = some_population.get_fittest_member();
                        self.member_fitness = member.get_fitness();
                        self.member_cells = member.get_cells();
                    }
                    self.generation += 1;
                    println!("{}", self.generation);
                    new_population = Some(self.ga.new_generation(&mut some_population, &mut self.experiment));
                }

                self.population = new_population;
            },

            _ => { }
        }
    }

    fn mouse_clicked(&mut self, mouse: mouse::MouseButton, cursor: [f64; 2]) {
        let x = ((cursor[0] - LEFT_MARGIN) / CELL_SIZE) as i32;
        let y = ((cursor[1] - TOP_MARGIN) / CELL_SIZE) as i32;
        if x >= 0 && y >= 0 && ((x as usize) < WORLD_SIZE) && ((y as usize) < WORLD_SIZE) {
            let model = self.experiment.get_model();
            model[x as usize][y as usize] = !model[x as usize][y as usize];
        }
    }
}

fn main() {
    start_app();
}

fn start_app() {
    let opengl = OpenGL::V2_1;

    let mut window: Window = WindowSettings::new("cell", [1024, 768]).
        exit_on_esc(true).
        opengl(opengl).
        build().
        unwrap();
    
    let font_path = match OpenOptions::new().open("FiraMono-Bold.ttf") {
        Ok(_) => Path::new("FiraMono-Bold.ttf"),
        Err(_) => {
            match OpenOptions::new().open("src/FiraMono-Bold.ttf") {
                Ok(_) => Path::new("src/FiraMono-Bold.ttf"),
                Err(_) => panic!("Font file is missing, or does not exist in the current path."),
            }
        }
    };

    let mut app = App {
        gl: GlGraphics::new(opengl),
        cache: GlyphCache::new(font_path).unwrap(),
        ga: ga::GA::new(),
        generation: 0,
        experiment: CellExperiment::new(),
        population: None,
        member_fitness: 0.0f32,
        member_cells: [[Cell::default(); WORLD_SIZE]; WORLD_SIZE],
    };  

    let mut events = window.events();
    let mut last_cursor = [0.0f64, 0.0f64];

    while let Some(e) = events.next(&mut window) {
        if let Some(Button::Keyboard(key)) = e.press_args() {
            app.key_pressed(key);
        };

        e.mouse_cursor(|x, y| {
            last_cursor = [x, y];
        });

        if let Some(Button::Mouse(button)) = e.press_args() {
            app.mouse_clicked(button, last_cursor);
        };

        if let Some(args) = e.render_args() {
            app.render(&args);
        };

        e.update(|args| { app.update(&args); });
    }
}