#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include "hex_to_bin.h"
#define F first
#define S second
#define PB push_back
#define CAN const auto &
#define REP(i,a,b) for(int i{a}; i <= b; ++i)
#define enl "\n"
#define dbg(x) cout << "(dbg) " << x << enl

using namespace __gnu_pbds;
using namespace std;

using indexed_set = tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update>;
using ll = long long;
using vi = vector<int>;
using vl = vector<ll>;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using pss = pair<std::size_t, std::size_t>;

constexpr std::size_t VERSION_SIZE{3};
constexpr std::size_t PACKET_ID_SIZE{3};
constexpr std::size_t LITERAL_DATA_SIZE{4};
constexpr std::size_t LENGTH_TYPE_0_SIZE{15};
constexpr std::size_t LENGTH_TYPE_1_SIZE{11};

int parse_packet(istreambuf_iterator<char> &it, ll &bits_parsed);

inline int parse_length_type_0(istreambuf_iterator<char> &it, ll &bits_parsed) {
	string length_string;
	copy_n(it, LENGTH_TYPE_0_SIZE, back_inserter(length_string));
	++it;
	bits_parsed += LENGTH_TYPE_0_SIZE;
	return stoi(length_string, nullptr, 2);
}

inline int parse_length_type_1(istreambuf_iterator<char> &it, ll &bits_parsed) {
	string length_string;
	copy_n(it, LENGTH_TYPE_1_SIZE, back_inserter(length_string));
	++it;
	bits_parsed += LENGTH_TYPE_1_SIZE;
	return stoi(length_string, nullptr, 2);
}

int parse_operator(istreambuf_iterator<char> &it, ll &bits_parsed) {
	int version{0};
	char length_type_id{*it++};
	bits_parsed += 1;
	switch (length_type_id) {
		case '0': {
			int total_length{parse_length_type_0(it, bits_parsed)};
			ll total_size{total_length + bits_parsed};
			while (bits_parsed < total_size) {
				version += parse_packet(it, bits_parsed);
			}
		}
		break;
		case '1': {
			int num_packets{parse_length_type_1(it, bits_parsed)};
			for (int i{0}; i < num_packets; ++i) {
				version += parse_packet(it, bits_parsed);
			}
		}
	}
	return version;
}

ll parse_literal(istreambuf_iterator<char> &it, ll &bits_parsed) {
	char is_last{'1'};
	string four_bits{};
	unsigned long long n{0};
	while (is_last == '1') {
		is_last = *it++;
		bits_parsed += 1;
		// cout << "bits_parsed: " << bits_parsed << enl;
		copy_n(it, LITERAL_DATA_SIZE, back_inserter(four_bits));
		++it;
		bits_parsed += LITERAL_DATA_SIZE;
		n = (n << LITERAL_DATA_SIZE) | stoll(four_bits, nullptr, 2);
		four_bits.clear();
	}
	// cout << "literal: " << n << enl;
	return n;
}

int parse_packet(istreambuf_iterator<char> &it, ll &bits_parsed) {
	int version;
	{
		string buffer;
		copy_n(it, VERSION_SIZE, back_inserter(buffer));
		++it;
		bits_parsed += VERSION_SIZE;
		version = stoi(buffer, nullptr, 2);
	}
	int packet_id;
	{
		string buffer;
		copy_n(it, PACKET_ID_SIZE, back_inserter(buffer));
		++it;
		bits_parsed += PACKET_ID_SIZE;
		packet_id = stoi(buffer, nullptr, 2);
	}
	// cout << "version: " << version << " packet_id: " << packet_id << enl;
	switch (packet_id) {
		case 4:
		parse_literal(it, bits_parsed);
		break;
		default:
		version += parse_operator(it, bits_parsed);
	}
	return version;
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	boost::iostreams::filtering_istream in{hex_to_bin_filter{}};
	in.push(cin);
	istreambuf_iterator<char> it{in};
	ll bits_parsed{0};
	cout << parse_packet(it, bits_parsed) << enl;
