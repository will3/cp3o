#pragma once

typedef glm::vec3 float3;

struct Coord3 {
	int i;
	int j;
	int k;

	Coord3() {};
	Coord3(int i, int j, int k) : i(i), j(j), k(k) {};

	bool operator == (const Coord3 &o) const {
		return i == o.i && j == o.j && k == o.k;
	}

	bool operator != (const Coord3 &o) const {
		return i != o.i || j != o.j || k != o.k;
	}

	bool operator < (const Coord3 &o) const {
		if (i == o.i) {
			if (j == o.j) {
				return k < o.k;
			}
			return j < o.j;
		}
		return i < o.i;
	}

	Coord3 operator - (const Coord3 &o) const {
        return Coord3( i - o.i, j - o.j, k - o.k );
	}

	Coord3 operator + (const Coord3 &o) const {
        return Coord3( i + o.i, j + o.j, k + o.k );
	}

	Coord3 operator * (int num) const {
        return Coord3( i * num, j * num, k * num );
	}

	Coord3 operator / (int num) const {
        return Coord3( i / num, j / num, k / num );
	}

	int& operator[] (const int index) {
		if (index == 0) {
			return i;
		}
		else if (index == 1) {
			return j;
		}
		return k;
	}

	Coord3 rotate(int d) {
		if (d == 0) {
            return Coord3( i, j, k );
		}
		else if (d == 1) {
            return Coord3( k, i, j );
		}
        return Coord3( j, k, i );
	}
};

typedef Coord3 chunk_key;

namespace std {
	template <>
	struct hash<Coord3>
	{
		std::size_t operator()(const Coord3& k) const
		{
			using std::hash;

			std::size_t v = 23;
			
			v += hash<int>()(k.i) * 17;
			v += hash<int>()(k.j) * 17;
			v += hash<int>()(k.k) * 17;

			return v;
		}
	};
}
