#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <functional>

template <typename T>
class Repository {
public:
    void add(const T& value) {
        data_.push_back(value);
    }

    template <typename Pred>
    void remove_if(Pred pred) {
        data_.erase(
            std::remove_if(data_.begin(), data_.end(), pred),
            data_.end()
        );
    }

    template <typename Pred>
    std::vector<T> find_if(Pred pred) const {
        std::vector<T> result;
        for (const auto& x : data_) {
            if (pred(x)) {
                result.push_back(x);
            }
        }
        return result;
    }

    void save_to_file(const std::string& filename) const {
        std::ofstream out(filename, std::ios::binary);
        std::size_t n = data_.size();
        out.write(reinterpret_cast<const char*>(&n), sizeof(n));
        out.write(reinterpret_cast<const char*>(data_.data()),
            n * sizeof(T));
    }

    void load_from_file(const std::string& filename) {
        std::ifstream in(filename, std::ios::binary);
        std::size_t n{};
        in.read(reinterpret_cast<char*>(&n), sizeof(n));
        data_.resize(n);
        in.read(reinterpret_cast<char*>(data_.data()),
            n * sizeof(T));
    }

    const std::vector<T>& data() const {
        return data_;
    }

private:
    std::vector<T> data_;
};
#include <iostream>

int main() {
    Repository<int> repo;

    repo.add(1);
    repo.add(2);
    repo.add(3);

    repo.save_to_file("data.bin");

    Repository<int> repo2;
    repo2.load_from_file("data.bin");

    auto found = repo2.find_if([](int x) { return x % 2 == 1; });

    for (int x : found) {
        std::cout << x << ' ';
    }
}
