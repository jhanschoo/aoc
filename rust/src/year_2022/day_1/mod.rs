use std::io::{BufRead, Error, Write};

pub fn day_1_1(input: &mut impl BufRead, output: &mut impl Write) -> Result<(), Error> {
    let (tentative_max, curr) = input
        .lines()
        .fold((0u64, 0u64), |(max_cal, curr_cal), l| {
            let l = l.unwrap();
            if l.is_empty() {
                (max_cal.max(curr_cal), 0)
            } else {
                (max_cal, curr_cal + l.parse::<u64>().unwrap())
            }
        });
    let max_cal = tentative_max.max(curr);
    writeln!(output, "{max_cal}")?;
    Ok(())
}

#[cfg(test)]
mod test {
    use std::{io::{self, BufReader}, fs::File};
    #[test]
    fn diagnostic_day_1_1() {
        let input = File::open("src/year_2022/day_1/diagnostic.txt").unwrap();
        let mut input = BufReader::new(input);
        super::day_1_1(&mut input, &mut io::stdout()).unwrap();
    }

    #[test]
    fn test_day_1_1() {
        let input = File::open("src/year_2022/day_1/input.txt").unwrap();
        let mut input = BufReader::new(input);
        super::day_1_1(&mut input, &mut io::stdout()).unwrap();
    }
}
