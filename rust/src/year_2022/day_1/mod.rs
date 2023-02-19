use std::{io::{BufRead, Error}, cmp};

pub fn day_1_1(input: &mut impl BufRead) -> Result<String, Error> {
    let (tentative_max, curr) = input
        .lines()
        .filter_map(Result::ok)
        .fold((0, 0), |(max_cal, curr_cal), l| {
            if l.is_empty() {
                (max_cal.max(curr_cal), 0)
            } else {
                (max_cal, curr_cal + l.parse::<i32>().unwrap())
            }
        });
    let tentative_max = tentative_max.max(curr);
    Ok(format!("{tentative_max}"))
}

pub fn day_1_2(input: &mut impl BufRead) -> Result<String, Error> {
    let mut maximums = vec![0; 3];
    let mut curr = 0;
    let mut update_maximums = |v| {
        let max_elem = maximums.iter_mut().min().unwrap();
        *max_elem = cmp::max(*max_elem, v);
    };

    input
        .lines()
        .filter_map(Result::ok)
        .for_each(|l| {
            if l.is_empty() {
                update_maximums(curr);
                curr = 0;
            } else {
                curr += l.parse::<i32>().unwrap();
            }
        });
    update_maximums(curr);
    let sum_cal: i32 = maximums.iter().sum();
    Ok(format!("{sum_cal}"))
}

#[cfg(test)]
mod test {
    use std::{io::{self, BufReader, Write}, fs::File};
    #[test]
    fn diagnostic_day_1_1() {
        let input = File::open("src/year_2022/day_1/diagnostic.txt").unwrap();
        let mut input = BufReader::new(input);
        let res = super::day_1_1(&mut input).unwrap();
        assert_eq!(res, "24000");
        writeln!(io::stdout(), "{}", res).unwrap();
    }

    #[test]
    fn test_day_1_1() {
        let input = File::open("src/year_2022/day_1/input.txt").unwrap();
        let mut input = BufReader::new(input);
        let res = super::day_1_1(&mut input).unwrap();
        writeln!(io::stdout(), "{}", res).unwrap();
    }

    #[test]
    fn diagnostic_day_1_2() {
        let input = File::open("src/year_2022/day_1/diagnostic.txt").unwrap();
        let mut input = BufReader::new(input);
        let res = super::day_1_2(&mut input).unwrap();
        assert_eq!(res, "45000");
        writeln!(io::stdout(), "{}", res).unwrap();
    }

    #[test]
    fn test_day_1_2() {
        let input = File::open("src/year_2022/day_1/input.txt").unwrap();
        let mut input = BufReader::new(input);
        let res = super::day_1_2(&mut input).unwrap();
        writeln!(io::stdout(), "{}", res).unwrap();
    }
}
