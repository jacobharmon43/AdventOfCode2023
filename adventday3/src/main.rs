use std::fs::File;
use std::io::{self, BufRead};

fn main() -> io::Result<()> {
    let filename: String = "src/input.txt".to_string();
    let file: File = File::open(filename)?; // If open is error, halt program

    let reader = io::BufReader::new(file);

    const LINE_LEN: usize = 140; // number of characters in input line

    // Initialize empty buffers for processing lines
    let mut top_row = LineData::new(LINE_LEN + 2); // add two for left/right buffers
    let mut mid_row = LineData::new(LINE_LEN + 2);
    let mut low_row = LineData::new(LINE_LEN + 2);

    let mut part1_count: u32 = 0;
    let mut part2_count: u32 = 0;

    for (_line_number, line) in reader.lines().enumerate() {
        let line = line?; // If line is error, halt program

        let mut next_line = LineData::new(LINE_LEN + 2);
        next_line.line_buff = format!(".{}.", line); // add left/right buffers for easier parsing

        // Process Middle line
        let (part1_increment, part2_increment) = process_line(&mut mid_row, &top_row, &mut low_row);

        part1_count += part1_increment;
        part2_count += part2_increment;

        // shift window of lines to process
        top_row = mid_row;
        mid_row = low_row;
        low_row = next_line;
    }

    // Handle last lines
    // Pack lower lines in window with default buffer
    for _i in 0..2 {
        let final_buff = LineData::new(LINE_LEN + 2);

        let (part1_increment, part2_increment) = process_line(&mut mid_row, &top_row, &mut low_row);

        part1_count += part1_increment;
        part2_count += part2_increment;

        top_row = mid_row;
        mid_row = low_row;
        low_row = final_buff;
    }

    println!("Part one value: {}", part1_count);
    println!("Part two value: {}", part2_count);

    Ok(())
}

/// Takes the window of three lines as input.
/// Locates and identifies all numbers in bottom row of window.
/// Identifies all part one numbers in middle row
/// Identifies all valid gears in middle row for part two
///
/// # Returns
///
/// (part1_increment: u32, part2_increment: u32)
fn process_line(mid: &mut LineData, above: &LineData, below: &mut LineData) -> (u32, u32) {
    let mut num: u32 = 0;
    let mut location: Vec<usize> = Vec::new();

    // Find all numbers in line
    for (index, c) in below.line_buff.char_indices() {
        if c.is_digit(10) {
            // save index of digit
            location.push(index);
            // store digit in a number
            let digit = c.to_digit(10).unwrap_or(0);
            num = (num * 10) + digit;
        } else {
            // Number has been found
            if num != 0 {
                // Store number and the location of its digits
                let element = Numbers {
                    number: num,
                    positions: location.clone(),
                };

                below.content.push(element);

                // Clear values for next number to be found
                num = 0;
                location.clear();
            }

            // Find all gears for part two
            if c == '*' {
                below.gear_pos.push(index);
            }
        }
    }

    // Sum up all valid numbers for part one
    let mut sum: u32 = 0;

    for element in &mid.content {
        let (left, right) = element.get_boundary();

        // Check above line
        let mut is_valid: bool = process_range(&above.line_buff, left - 1, right + 1);

        // Check neighbors
        if is_valid == false {
            is_valid = process_range(&mid.line_buff, left - 1, right + 1);
        }

        // Check below line
        if is_valid == false {
            is_valid = process_range(&below.line_buff, left - 1, right + 1);
        }

        // Add number if valid
        if is_valid {
            sum += element.number;
        }
    }

    // For part two
    let mut gear_sum: u32 = 0;

    // Check any gears on middle line for valid neighbors
    for gear in &mid.gear_pos {
        if let Some(num_counter) = find_neighbors(gear, &mid, &above, &below) {
            if num_counter.len() == 2 {
                gear_sum += num_counter[0] * num_counter[1];
            }
        }
    }
    (sum, gear_sum)
}

/// Takes the position of a single gear in the middle line of the window.
/// Finds all valid neighbors of that gear.
///
/// # Returns
/// Option(Vec<u32>): valid neighbors
fn find_neighbors(
    gear: &usize,
    target: &LineData,
    above: &LineData,
    below: &LineData,
) -> Option<Vec<u32>> {
    let mut neighbors: Vec<u32> = Vec::new();

    for num in &above.content {
        if num.positions.contains(&(gear - 1))
            || num.positions.contains(gear)
            || num.positions.contains(&(gear + 1))
        {
            neighbors.push(num.number.clone());
        }
    }

    for num in &target.content {
        if num.positions.contains(&(gear - 1)) || num.positions.contains(&(gear + 1)) {
            neighbors.push(num.number.clone());
        }
    }

    for num in &below.content {
        if num.positions.contains(&(gear - 1))
            || num.positions.contains(gear)
            || num.positions.contains(&(gear + 1))
        {
            neighbors.push(num.number.clone());
        }
    }

    // Return options
    if neighbors.is_empty() {
        None
    } else {
        Some(neighbors)
    }
}

fn is_symbol(item: &char) -> bool {
    !item.is_digit(10) && *item != '.'
}

/// Looks for symbols across a defined range in a single line
fn process_range(line: &str, left: usize, right: usize) -> bool {
    let mut symbol_found: bool = false;
    for (_i, c) in line.char_indices().skip(left).take(right - left + 1) {
        if is_symbol(&c) {
            symbol_found = true;
            break;
        }
    }

    symbol_found
}

/// pack lines with '.' character to the same length as inputs
/// Used for borders on the input data
fn pack_line(len: usize) -> String {
    std::iter::repeat('.').take(len).collect()
}

struct LineData {
    line_buff: String,
    content: Vec<Numbers>,
    gear_pos: Vec<usize>,
}

impl LineData {
    fn new(len: usize) -> Self {
        Self {
            line_buff: pack_line(len),
            content: Vec::new(),
            gear_pos: Vec::new(),
        }
    }
}

/// Numbers should store the value and the indices of all of their digits
struct Numbers {
    number: u32,
    positions: Vec<usize>,
}

impl Numbers {
    /// Find index for first and last digit in number
    fn get_boundary(&self) -> (usize, usize) {
        let left = *self.positions.iter().min().unwrap_or(&0);
        let right = *self.positions.iter().max().unwrap_or(&0);

        (left, right)
    }
}