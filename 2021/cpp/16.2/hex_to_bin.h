#include <iosfwd>
#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/char_traits.hpp>

class hex_to_bin_filter : public boost::iostreams::input_filter {
	public:
		template<typename Source>
		char get(Source& src) {
			if (buffer_seen < 4) {
				++buffer_seen;
				return ((buffer >> (4 - buffer_seen)) & 1) + '0';
			}
			char c;
			if ((c = boost::iostreams::get(src)) == EOF || c == boost::iostreams::WOULD_BLOCK) {
				return c;
			}
			// std::cout << "c: " << c << std::endl;
			buffer = stoi(std::string{c}, nullptr, 16);
			buffer_seen = 1;
			return ((buffer >> 3) & 1) + '0';
		}

		// template<typename Source>
		// std::streamsize read(Source& src, char* s, std::streamsize n) {
		// 	std::streamsize result{0};
		// 	while (true) {
		// 		for (; result < n && buffer_seen < 4; ++buffer_seen, ++result) {
		// 			*s++ = ((buffer >> (3 - buffer_seen)) & 1) + '0';
		// 		}
		// 		if (result == n) {
		// 			return result;
		// 		}
		// 		char c;
		// 		if ((c = boost::iostreams::get(src)) == EOF) {
		// 			return result == 0 ? -1 : result;
		// 		} else if (c == boost::iostreams::WOULD_BLOCK) {
		// 			return result;
		// 		}
		// 		buffer = stoi(std::string{c}, nullptr, 16);
		// 		buffer_seen = 0;
		// 	}
		// }

		template<typename Source>
		void close(Source&) { buffer_seen = 4; }
	private:
		int buffer;
		int buffer_seen{4};
};