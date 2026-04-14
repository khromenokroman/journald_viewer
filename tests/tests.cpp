#include <gtest/gtest.h>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>

static std::string read_file(std::filesystem::path const &path) {
    std::ifstream in(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
}

TEST(JournaldViewerIntegrationTest, OutputMatchesExpected) {
    const auto current = std::filesystem::current_path();
    const auto input = current / "tests" / "journald";
    const auto expected = current / "tests" / "journald.expected";
    const auto out = current / "tests" / "out";

    const auto command = "cat \"" + input.string() + "\" | ./journald-viewer > \"" + out.string() + "\"";
    ASSERT_EQ(std::system(command.c_str()), 0);

    const auto actual_content = read_file(out);
    const auto expected_content = read_file(expected);

    EXPECT_EQ(actual_content, expected_content);
}
