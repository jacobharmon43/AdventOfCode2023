fn main() {
    use std::fs::read_to_string;
    let mut sum: u128 = 0;
    let mut index: usize = 1;
    let mut count: u128 = 0;
    let mut prev_score: u128 = 1;
    let lines: String = read_to_string("src/sampleinput.txt").unwrap();
    let original_len: usize = lines.lines().count();
    let mut score_vec: Vec<u128> = Vec::new();
    for i in 0..original_len {
        score_vec.push(1);
    } 
    for line in lines.lines() {
        let score = line_score(line.to_string());
        let mut local_count = 0;
        for i in index-1..index+(score as usize)-1 {
            if i >= original_len { break; }
            local_count += 1;
            score_vec[index + i - 1] += 1;
        }
        count += score_vec[index] * local_count;
        println!("card {index} score {score} new count {count}");
        index+=1;
    }
    count += original_len as u128;
    println!("{count}");
}

fn line_score(line: String) -> u128 {
    let new_start = line.find(':').unwrap() + 1;
    let modified_input: String = line[new_start..].to_string();
    let number_sets: Vec<&str> = modified_input.split('|').collect();
    let winning_numbers: Vec<&str> = number_sets[0].split(" ").collect();
    let card_numbers: Vec<&str> = number_sets[1].split(" ").collect();
    let mut matches = 0;
    let mut score = 0;
    for number in card_numbers {
        if number.trim().len() == 0 { continue; }
        if winning_numbers.contains(&&number) {
            score += 1;
            matches+=1;
        }
    }
    return score as u128;
}
