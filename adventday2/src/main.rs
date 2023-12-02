use std::fmt::format;

fn main() {
    use std::fs::read_to_string;
    let mut sum: u32 = 0;
    let mut index: u8 = 1;
    for line in read_to_string("src/input.txt").unwrap().lines() {
        sum += game_score(line, index);
        index+=1;
    }
    println!("{sum}");
}

fn possible_game(input: &str, index: u8, red_max: u32, blue_max: u32, green_max: u32) -> bool {
    let modified_input: String = input.replace(&format!("Game {0}: ", index), "");
    let showings: Vec<&str> = modified_input.split(';').collect();
    for show in showings {
        let colors: Vec<&str> = show.split(',').collect();
        for color in colors {
            let strs: Vec<&str> = color.trim().split(" ").collect();
            let number: u32 = strs[0].trim().parse::<u32>().unwrap();
            println!("{number} vs {color}");
            match color_match(color) {
                Some("red") => if number > red_max { return false }
                Some("blue") => if number > blue_max { return false }
                Some("green") => if number > green_max { return false }
                None => println!("Error"),
                _ => {}
            }
        }
    }
    return true;
}

fn game_score(input: &str, index: u8) -> u32 {
    let modified_input: String = input.replace(&format!("Game {0}: ", index), "");
    let showings: Vec<&str> = modified_input.split(';').collect();
    let mut max_red_shown: u32 = 0;
    let mut max_blue_shown: u32 = 0;
    let mut max_green_shown: u32 = 0;
    for show in showings {
        let colors: Vec<&str> = show.split(',').collect();
        for color in colors {
            let strs: Vec<&str> = color.trim().split(" ").collect();
            let number: u32 = strs[0].trim().parse::<u32>().unwrap();
            match color_match(color) {
                Some("red") => if number > max_red_shown { max_red_shown = number }
                Some("blue") => if number > max_blue_shown { max_blue_shown = number }
                Some("green") => if number > max_green_shown { max_green_shown = number }
                None => println!("Error"),
                _ => {}
            }
        }
    }
    return max_red_shown * max_blue_shown * max_green_shown;
}

fn color_match(substring: &str) -> Option<&str> {
        if substring.contains("red") {
        Some("red")
    } else if substring.contains("blue") {
        Some("blue")
    } else if substring.contains("green") {
        Some("green")
    } else {
        None
    }
}