#pragma once
#include <memory>
#include <iostream>
#include <string>

namespace snailfish_impl {
	class sf_base;
	class sf_regular;
	class sf_pair;
	class sf_base {
	public:
		virtual std::tuple<bool, std::pair<bool, long long>, std::pair<bool, long long>> explode(int depth) = 0;
		virtual std::tuple<bool, std::shared_ptr<sf_pair>> split() = 0;
		virtual long long get_val() const = 0;
		virtual void add_first_val(long long val) = 0;
		virtual void add_second_val(long long val) = 0;
		virtual std::ostream &print(std::ostream &os) const = 0;
		friend std::ostream &operator<<(std::ostream &os, const sf_base &s) {
			return s.print(os);
		}
	};

	class sf_regular: public sf_base {
		public:
			sf_regular(long long val): val_{val} {};
			std::tuple<bool, std::pair<bool, long long>, std::pair<bool, long long>> explode(int depth) override { return {false, {false, 0}, {false, 0}}; };
			std::tuple<bool, std::shared_ptr<sf_pair>> split() override;
			long long get_val() const override { return val_; }
			void add_first_val(long long val) override { val_ += val; }
			void add_second_val(long long val) override { val_ += val; }
			std::ostream &print(std::ostream &os) const override { return os << val_; }
		private:
			long long val_;
	};

	std::tuple<bool, std::shared_ptr<sf_pair>> sf_regular::split() {
		if (val_ >= 10) {
			auto lower{val_ / 2};
			return {true, std::make_shared<sf_pair>(std::make_shared<sf_regular>(lower), std::make_shared<sf_regular>(val_ - lower))};
		} else {
			return {false, nullptr};
		}
	}

	class sf_pair: public sf_base {
		public:
			sf_pair(std::shared_ptr<sf_base> first, std::shared_ptr<sf_base> second): first_{first}, second_{second} {};
			std::tuple<bool, std::pair<bool, long long>, std::pair<bool, long long>> explode(int depth) override;
			std::tuple<bool, std::shared_ptr<sf_pair>> split() override;
			long long get_val() const override { return 3 * first_->get_val() + 2 * second_->get_val(); }
			void add_first_val(long long val) override { first_->add_first_val(val); }
			void add_second_val(long long val) override { second_->add_second_val(val); }
			std::ostream &print(std::ostream &os) const override { return os << '[' << *first_ << ',' << *second_ << ']'; }
		private:
			std::shared_ptr<sf_base> first_;
			std::shared_ptr<sf_base> second_;
			bool reduce(int depth);
	};

	std::tuple<bool, std::pair<bool, long long>, std::pair<bool, long long>> sf_pair::explode(int depth) {
		if (depth == 4) {
			return {true, {true, first_->get_val()}, {true, second_->get_val()}};
		}
		{
			auto [exploded, first_prop, second_prop] = first_->explode(depth + 1);
			if (exploded) {
				if (first_prop.first && second_prop.first) {
					first_ = std::make_shared<sf_regular>(0);
				}
				auto [prop, prop_val] = second_prop;
				if (prop) {
					second_->add_first_val(prop_val);
				}
				return {true, first_prop, {false, 0}};
			}
		}
		{
			auto [exploded, first_prop, second_prop] = second_->explode(depth + 1);
			if (exploded) {
				if (first_prop.first && second_prop.first) {
					second_ = std::make_shared<sf_regular>(0);
				}
				auto [prop, prop_val] = first_prop;
				if (prop) {
					first_->add_second_val(prop_val);
				}
				return {true, {false, 0}, second_prop};
			}
		}
		return {false, {false, 0}, {false, 0}};
	}

	std::tuple<bool, std::shared_ptr<sf_pair>> sf_pair::split() {
		for (auto childp : {&first_, &second_}) {
			auto [splitted, pairp] = (*childp)->split();
			if (splitted) {
				if (pairp != nullptr) {
					*childp = pairp;
				}
				return {true, nullptr};
			}
		}
		return {false, nullptr};
	}

	std::shared_ptr<sf_base> from_istream(std::istream &is) {
		if (is.peek() == '[') {
			is.get(); // '['
			auto first = from_istream(is);
			is.get(); // ','
			auto second = from_istream(is);
			is.get(); // ']'
			return std::make_shared<sf_pair>(first, second);
		} else {
			long long val;
			is >> val;
			return std::make_shared<sf_regular>(val);
		}
	}
}

class snailfish {
	public:
	snailfish(long long val): sf_{std::make_shared<snailfish_impl::sf_regular>(val)} {}
	snailfish(std::shared_ptr<snailfish_impl::sf_base> sf): sf_{sf} {}
	snailfish(std::istream& is): sf_{snailfish_impl::from_istream(is)} {}
	snailfish operator+(const snailfish &other) const {
		auto ret_sf_{std::make_shared<snailfish_impl::sf_pair>(sf_, other.sf_)};
		while (true) {
			auto [exploded, _1, _2] = ret_sf_->explode(0);
			if (exploded) {
				continue;
			}
			auto [splitted, _3] = ret_sf_->split();
			if (splitted) {
				continue;
			}
			break;
		}
		return snailfish{ret_sf_};
	}
	long long get_magnitude() const { return sf_->get_val(); }
	friend std::ostream &operator<<(std::ostream &os, const snailfish &s) {
		return os << *s.sf_;
	}
	friend std::istream &operator>>(std::istream &is, snailfish &s) {
		s.sf_ = snailfish_impl::from_istream(is);
		return is;
	}
	private:
	std::shared_ptr<snailfish_impl::sf_base> sf_;
};
