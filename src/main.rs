extern crate piston;
extern crate graphics;
extern crate glutin_window;
extern crate opengl_graphics;
extern crate rand;

/*pub mod ga;
pub mod network;
pub mod cell;
pub mod xor_network;*/

use piston::window::WindowSettings;
use piston::event::*;
use glutin_window::GlutinWindow as Glutin_Window;
use opengl_graphics::{ GlGraphics, OpenGL };
use opengl_graphics::glyph_cache::GlyphCache;
use graphics::{ Transformed };
use piston::input::Button::{ Keyboard };
use piston::input::Input;
use piston::input::keyboard::Key;
use std::path::Path;
use std::env;

//use ga::*;
//use xor_network::*;

//const WHITE: [f32; 4] = [1.0, 1.0, 1.0, 1.0];
//const BLACK: [f32; 4] = [0.0, 0.0, 0.0, 1.0];
//const LIGHT_GRAY: [f32; 4] = [0.3, 0.3, 0.3, 1.0];
//const DARK_GRAY: [f32; 4] = [0.1, 0.1, 0.1, 1.0];
//const GREEN: [f32; 4] = [0.0, 1.0, 0.0, 1.0];
//const CYAN: [f32; 4] = [0.0, 1.0, 1.0, 1.0];
//const RED: [f32; 4] = [1.0, 0.0, 0.0, 1.0];
//const BLUE: [f32; 4] = [0.0, 0.0, 1.0, 1.0];
//const ORANGE: [f32; 4] = [1.0, 0.5, 0.0, 1.0];
//const YELLOW: [f32; 4] = [1.0, 1.0, 0.0, 1.0];
//const PURPLE: [f32; 4] = [0.5, 0.0, 1.0, 1.0];

const TEXT_FONT_SIZE: u32 = 22;
const CELL_SIZE: f64 = 30.0;
const LEFT_MARGIN: f64 = 20f64;
const TOP_MARGIN: f64 = 30f64;

struct App {
    gl: GlGraphics,
    cache: GlyphCache<'static>,
}

struct Render;

impl Render {
}

impl App {
    fn render(&mut self, args: &RenderArgs) {
    }

    fn update(&mut self, args: &UpdateArgs) {
    }

    fn handle_input(&mut self, i: Input) {
    }
}

fn main() {
    /*let mut experiment = XORNetworkExperiment::new();
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
    println!("Finished!");*/

    let opengl = OpenGL::_3_2;

    let window = Glutin_Window::new(
        opengl,
        WindowSettings::new("Piston Tetris", [1024, 768])
                       .exit_on_esc(true)
                       .vsync(true)
                       //TODO causes error on ubuntu
                       //.samples(1)
    );
    
    let font_path = Path::new("FiraMono-Bold.ttf");

    let p = env::current_dir().unwrap();
    println!("The current directory is {}", p.display());

    let mut app = App {
        gl: GlGraphics::new(opengl),
        cache: GlyphCache::new(font_path).unwrap(),
    };  

    for e in window.events().max_fps(60).ups(120) {
        match e {
            Event::Input(i) => {
                app.handle_input(i);
            }

            Event::Render(args) if app.should_redraw => {
                app.render(&args);
            }

            Event::Update(args) => {
                app.update(&args);
            }

            _ => {}
        } 
    }
}