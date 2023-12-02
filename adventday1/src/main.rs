use std::ops::Index;

fn main() {
    use std::fs::read_to_string;
    let mut sum: u128 = 0;
    for line in read_to_string("src/input.txt").unwrap().lines() {
        sum += get_calibration_number(line) as u128
    }
}

fn get_calibration_number(string: &str) -> u8 {
    const WORD_SET: [&str; 9] = ["one", "two", "three", "four", "five", "six", "seven", "eight", "nine"];
    let mut first_char: String = "E".to_owned();
    let mut first_char_index: u128 = std::u128::MAX;
    let mut second_char: String = "E".to_owned();
    let mut second_char_index: u128 = std::u128::MAX;
    for (index, c) in string.chars().enumerate() {
        if c.is_numeric() {
            if first_char == "E" {
                first_char = c.to_string();
                first_char_index = index as u128;
            }
            second_char = c.to_string();
            second_char_index = index as u128;
        }
    }
    let mut first_word: u8 = 0;
    let mut first_word_index: u128 = std::u128::MAX;
    let mut second_word: u8 = 0;
    let mut second_word_index: u128 = std::u128::MAX;
    let mut second_word_true_index: u128 = std::u128::MAX;
    for (index, word) in WORD_SET.iter().enumerate() {
        if let Some(value) = string.find(word) {
            let value_as_u128 = value as u128;
            if value_as_u128 < first_word_index {
                first_word = (index as u8) + 1;
                first_word_index = value_as_u128;
            }
        }
    }
    let reversed: String = string.chars().rev().collect();
    for (index, word) in WORD_SET.iter().enumerate() {
        if let Some(value) = reversed.find(&word.chars().rev().collect::<String>()) {
            let value_as_u128 = value as u128;
            if value_as_u128 < second_word_index  {
                second_word = (index as u8) + 1;
                second_word_index = value_as_u128;
                second_word_true_index = (string.len() as u128) - (value_as_u128 + (word.len() as u128));
            }
        }
    }

    let mut code: String = "".to_owned();
    if first_word_index == std::u128::MAX {
        code.push_str(&first_char.to_string());
    } else if first_char_index == std::u128::MAX {
        code.push_str(&first_word.to_string())
    } else if first_word_index < first_char_index { 
        code.push_str(&first_word.to_string());
    } else {
        code.push_str(&first_char.to_string());
    }

    if second_word_index == std::u128::MAX {
        code.push_str(&second_char.to_string())
    } else if second_char_index == std::u128::MAX {
        code.push_str(&second_word.to_string())
    } else if second_word_true_index < second_char_index {
        code.push_str(&second_char.to_string())
    } else {
        code.push_str(&second_word.to_string());
    }
    return code.parse::<u8>().unwrap();
}
