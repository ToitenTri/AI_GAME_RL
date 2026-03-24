# Môi Trường Trò Chơi Thách Thức Thuật Toán AI - C++

Một khung làm việc C++ toàn diện, chuyên nghiệp để kiểm tra và so sánh các Agent AI trên các thách thức thuật toán. Đây là một sân chơi kiểm tra nghiêm ngặt để đánh giá kỹ năng lập trình thuật toán và giải quyết vấn đề của Agent AI.

## 🎮 Khái Niệm Trò Chơi

Trò chơi trình bày các thách thức thuật toán tuần tự mà Agent AI phải giải quyết một cách tối ưu, đồng thời bị phạt vì:
- **Inefficiency (Kém hiệu quả)**: Vượt quá giới hạn bước/thời gian
- **Invalid Moves (Nước đi không hợp lệ)**: Vi phạm quy tắc trò chơi
- **Resource Mismanagement (Quản lý tài nguyên kém)**: Vượt quá giới hạn bộ nhớ
- **Logic Failures (Lỗi logic)**: Đầu ra sai hoặc không đáp ứng điều kiện

Các Agent AI khác nhau có thể được nhúng vào để cạnh tranh với nhau.

### Các Loại Thách Thức

1. **Pathfinding (Tìm Đường)**: Dẫn hướng trên lưới 10×10 từ điểm đầu đến đích tránh chướng ngại vật
2. **Sorting (Sắp Xếp)**: Sắp xếp mảng với số lượng phép so sánh/hoán vị tối thiểu
3. **Resource Allocation (Phân Bổ Tài Nguyên)**: Phân bổ 100 đơn vị tài nguyên cho 5 nhiệm vụ một cách tối ưu

## 📋 Kiến Trúc Hệ Thống

```
┌─────────────────────────────────────────────────────────┐
│                   GAME ENGINE (Game.h)                  │
│                                                         │
│  ┌──────────────────┐  ┌──────────────────┐             │
│  │    Challenges    │  │    AI Agents     │             │
│  │   (Challenge.h)  │  │   (AIAgent.h)    │             │
│  │                  │  │                  │             │
│  │ • Pathfinding    │  │ • SimpleGreedy   │             │
│  │ • Sorting        │  │ • Optimized      │             │ 
│  │ • Resources      │  │ • Custom (user)  │             │
│  └──────────────────┘  └──────────────────┘             │
│         △                        △                     │
│         └────────────┬───────────┘                      │
│                      │                                  │
│          ┌─────────────────────────┐                    │
│          │  PENALTY SCOREBOARD     │                    │
│          │ (PenaltyScoreboard.h)   │                    │
│          │                         │                    │
│          │ • Tracking Lỗi          │                    │
│          │ • Tính Toán Phạt        │                    │
│          │ • Phân Tích Hiệu Năng   │                    │
│          └─────────────────────────┘                    │
└─────────────────────────────────────────────────────────┘
```

## 🛠️ Các Thành Phần Cốt Lõi

### 1. **PenaltyScoreboard** (`PenaltyScoreboard.h/cpp`)

Theo dõi và quản lý các hình phạt cho lỗi của Agent AI:

**Các Loại Lỗi:**
- **Algorithmic Inefficiency**: Vượt quá bước/tLimit
- **Invalid Moves**: Hành động vi phạm luật trò chơi
- **Resource Mismanagement**: Vi phạm giới hạn bộ nhớ
- **Logic Failures**: Đầu ra sai hoặc điều kiện thất bại
- **Time Limit Exceeded**: Chạy quá thời gian quy định

**Các Phương Thức Chính:**
```cpp
void recordPenalty(ErrorCategory, int points, std::string description, int levelId);
void penalizeAlgorithmicInefficiency(int stepsUsed, int stepsAllowed, int levelId);
void penalizeInvalidMove(const std::string& action, int levelId);
void penalizeResourceMismanagement(size_t memoryUsed, size_t limit, int levelId);
void penalizeLogicFailure(const std::string& reason, const std::string& expected, int levelId);
int getEfficiencyScore() const;  // Trả về 0-100
std::string generateReport() const;
```

### 2. **Challenge** (`Challenge.h/cpp`)

Lớp cơ sở trừu tượng định nghĩa giao diện cho các thách thức thuật toán:

```cpp
class Challenge {
    virtual void initialize() = 0;
    virtual bool executeAction(const std::string& action) = 0;
    virtual bool isSolved() const = 0;
    virtual std::string getState() const = 0;
    virtual std::string getExpectedResult() const = 0;

    // Theo dõi tích hợp
    void incrementStepCount();
    bool isStepLimitExceeded() const;
    void recordMemoryUsage(size_t bytes);
    bool isMemoryLimitExceeded() const;
};
```

### 3. **Các Thách Thức Cụ Thể** (`ConcreteChallenges.h/cpp`)

#### PathfindingChallenge (Thách Thức Tìm Đường)
```
Kích Thước Lưới: 10×10
Chướng Ngại Vật: 30% lưới
Hành Động: MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT
Mục Tiêu: Đến (9,9) từ (0,0)
Bước Tối Đa: 15 (khoảng cách Manhattan ~14)
Giới Hạn Bộ Nhớ: 1KB
```

#### SortingChallenge (Thách Thức Sắp Xếp)
```
Mảng: [64, 34, 25, 12, 22, 11, 90]
Hành Động: COMPARE x y, SWAP x y, VERIFY
Mục Tiêu: Sắp xếp với số phép toán tối thiểu
Bước Tối Đa: 50
Giới Hạn Bộ Nhớ: 2KB
```

#### ResourceAllocationChallenge (Thách Thức Phân Bổ Tài Nguyên)
```
Nhiệm Vụ: 5 với ràng buộc tài nguyên min/max
Tài Nguyên: 100 đơn vị tổng cộng
Hành Động: ALLOCATE task_id amount
Mục Tiêu: Phân bổ tối ưu tuân thủ ràng buộc
Bước Tối Đa: 20
Giới Hạn Bộ Nhớ: 4KB
```

### 4. **AIAgent** (`AIAgent.h/cpp`)

Giao diện cơ sở trừu tượng cho các Agent AI:

```cpp
class AIAgent {
public:
    virtual bool solveChallenge(Challenge& challenge,
                                PenaltyScoreboard& scoreboard) = 0;
    void recordCompletion(bool success, int stepsUsed, int penaltyPoints);
    AgentStats getStats() const;
};
```

**Các Triển Khai Mẫu:**

- **SimpleGreedyAgent**: Sử dụng chiến lược tham lam/heuristic
- **OptimizedAgent**: Sử dụng các thuật toán tinh vi hơn

### 5. **Game** (`Game.h/cpp`)

Công cụ trò chơi chính điều phối luồng chơi:

```cpp
class Game {
    void addChallenge(std::unique_ptr<Challenge> challenge);
    Game::SessionResult playSession(AIAgent& agent);
    bool playLevel(int levelIndex, AIAgent& agent);
    const PenaltyScoreboard& getScoreboard() const;
};
```

## 🏗️ Xây Dựng & Biên Dịch

### Sử Dụng CMake (Khuyến Nghị)

```bash
# Điều hướng đến thư mục dự án
cd d:\HOCTAP\IT\C++

# Tạo thư mục build
mkdir build
cd build

# Cấu hình dự án
cmake ..

# Biên dịch
cmake --build . --config Release

# Chạy tệp thực thi
./bin/ai_game_env    # Linux/Mac
.\bin\ai_game_env.exe  # Windows
```

### Biên Dịch Thủ Công (g++/clang)

```bash
# Biên dịch tất cả các tệp mã nguồn
g++ -std=c++17 -O2 -o ai_game_env \
    main.cpp \
    PenaltyScoreboard.cpp \
    Challenge.cpp \
    ConcreteChallenges.cpp \
    AIAgent.cpp \
    Game.cpp

# Chạy
./ai_game_env
```

### Sử Dụng MSVC (Windows)

```bash
cl /std:c++17 /EHsc /O2 *.cpp
ai_game_env.exe
```

### Single-Header (Nhanh Nhất)

```bash
g++ -std=c++17 -O2 main_single_header.cpp -o ai_game
./ai_game
```

## 📊 Hệ Thống Phạt Chi Tiết

### Tính Toán Kém Hiệu Quả

```
Nếu (stepsUsed > stepsAllowed):
    excess = stepsUsed - stepsAllowed
    penalty = min(50, (excess * 5) / stepsAllowed)  // Được cắn dài tối đa 50
```

**Ví Dụ:**
- Tìm Đường: 15 bước dùng vs 15 cho phép → 0 phạt ✓
- Tìm Đường: 18 bước dùng vs 15 cho phép → 1 phạt ⚠
- Tìm Đường: 22 bước dùng vs 15 cho phép → 2 phạt (cắn tối đa 50)

### Nước Đi Không Hợp Lệ
```
Hành Động Cố Gắng: "MOVE_OUTSIDE_BOUNDARY"
Phạt: 15 điểm (cố định)
```

### Quản Lý Tài Nguyên Kém
```
Giới Hạn Bộ Nhớ: 2048 bytes
Bộ Nhớ Dùng: 3072 bytes
Vượt Quá: 1024 bytes
Phạt: (1024 * 100) / 2048 = 50 điểm (cắn tối đa 40)
```

### Lỗi Logic
```
Mong Đợi: "Mảng sắp xếp theo thứ tự tăng dần"
Thực Tế: "Mảng không được sắp xếp"
Phạt: 30 điểm (cố định)
```

## 🎯 Ví Dụ Sử Dụng

### Tạo Trò Chơi và Chạy Một Phiên

```cpp
#include "Game.h"
#include "ConcreteChallenges.h"
#include "AIAgent.h"

int main() {
    // Tạo trò chơi
    Game::GameConfig config{"AI Challenge", 3, true};
    Game game(config);

    // Thêm thách thức
    Challenge::ChallengeConfig pathConfig{
        0, 15, 1024, "Pathfinding", "Điều hướng đến mục tiêu"
    };
    game.addChallenge(
        std::make_unique<PathfindingChallenge>(pathConfig, 10, 10)
    );

    // Tạo và kiểm tra Agent
    SimpleGreedyAgent agent;
    auto result = game.playSession(agent);

    std::cout << "Agent: " << result.agentName << std::endl;
    std::cout << "Hiệu Suất: " << result.efficiencyScore << "/100" << std::endl;

    return 0;
}
```

### Tạo Custom Agent

```cpp
class MyCustomAgent : public AIAgent {
public:
    MyCustomAgent() : AIAgent("MyAgent") {}

    bool solveChallenge(Challenge& challenge,
                       PenaltyScoreboard& scoreboard) override {
        // Thuật toán của bạn ở đây
        while (!challenge.isSolved()) {
            std::string action = computeNextAction(challenge);
            if (!challenge.executeAction(action)) {
                scoreboard.penalizeInvalidMove(action,
                    challenge.getConfig().levelId);
                return false;
            }
        }
        return true;
    }

private:
    std::string computeNextAction(Challenge& challenge) {
        // Logic AI của bạn
        return "MOVE_RIGHT";
    }
};
```

## 🔧 Hướng Dẫn Mở Rộng

### Thêm Loại Thách Thức Mới

```cpp
class MyChallenge : public Challenge {
public:
    MyChallenge(const ChallengeConfig& config)
        : Challenge(config) {}

    void initialize() override {
        // Thiết lập trạng thái ban đầu
    }

    bool executeAction(const std::string& action) override {
        if (!isValidMove(action)) return false;
        incrementStepCount();

        // Xử lý hành động và cập nhật trạng thái
        // ...

        // Kiểm tra điều kiện thắng
        if (/* đã giải quyết */) solved = true;
        return true;
    }

    bool isSolved() const override { return solved; }

    std::string getState() const override {
        return "Trạng thái câu đố hiện tại...";
    }

    std::string getExpectedResult() const override {
        return "Giải pháp mong đợi...";
    }

private:
    bool isValidMove(const std::string& action) const override {
        // Xác thực hành động
        return true;  // hoặc false nếu không hợp lệ
    }
};
```

### Tùy Chỉnh Trọng Số Phạt

```cpp
PenaltyScoreboard scoreboard;

// Làm cho phạt hiệu quả gấp 2 lần
scoreboard.setPenaltyWeight(
    PenaltyScoreboard::ErrorCategory::ALGORITHMIC_INEFFICIENCY, 2
);

// Làm cho nước đi không hợp lệ gặp phạt gấp 3 lần
scoreboard.setPenaltyWeight(
    PenaltyScoreboard::ErrorCategory::INVALID_MOVE, 3
);
```

## 📚 Cấu Trúc Tệp Dự Án

| Tệp | Mục Đích |
|------|---------|
| `PenaltyScoreboard.h/cpp` | Hệ thống theo dõi lỗi |
| `Challenge.h/cpp` | Lớp cơ sở của thách thức |
| `ConcreteChallenges.h/cpp` | Pathfinding, Sorting, Resource challenges |
| `AIAgent.h/cpp` | Lớp cơ sở và các triển khai Agent mẫu |
| `Game.h/cpp` | Công cụ trò chơi chính |
| `main.cpp` | Điểm vào và phiên trình diễn |
| `AIGameEnvironment.hpp` | Triển khai single-header (tất cả-trong-một) |
| `CMakeLists.txt` | Cấu hình CMake |
| `README_VI.md` | Tài liệu này |

## 🚀 Ý Tưởng Nâng Cao Trong Tương Lai

1. **Thách Thức Nâng Cao**: Vấn đề đồ thị, thoả mãn ràng buộc, tích hợp máy học
2. **Kiểm Tra Phân Tán**: Chạy nhiều Agent song song
3. **Hệ Thống Hiển Thị**: Bảng điều khiển GUI theo thời gian thực
4. **Tích Hợp Máy Học**: Các Agent mạng thần kinh
5. **Phân Tích Hiệu Năng**: Phân tích bộ nhớ/CPU chi tiết
6. **Chế Độ Giải Đấu**: Cạnh tranh đa Agent với xếp hạng
7. **Hệ Thống Phát Lại**: Ghi nhận và phân tích quyết định Agent
8. **Bộ Kiểm Tra Chuẩn**: Các trường hợp kiểm tra tiêu chuẩn

## ✨ Tính Năng Chính

✓ **Theo Dõi Phạt Toàn Diện**
- 5 loại lỗi với theo dõi độc lập
- Trọng số phạt tùy chỉnh
- Ghi nhật ký sự kiện có dấu thời gian
- Tính toán điểm hiệu suất

✓ **Nhiều Loại Thách Thức**
- Tìm đường với chướng ngại vật
- Tối ưu hóa sắp xếp mảng
- Phân bổ tài nguyên có ràng buộc
- Khung thách thức mở rộng

✓ **Khung AI Hoàn Chỉnh**
- Giao diện trừu tượng cho Agent tùy chỉnh
- Theo dõi thống kê tự động
- Các triển khai mẫu tham lam và tối ưu
- Khả năng so sánh hiệu suất

✓ **Triển Khai C++ Hiện Đại**
- Các tính năng C++17 và best practices
- Quản lý bộ nhớ con trỏ thông minh
- Các vùng chứa và thuật toán STL
- Nguyên tắc RAII xuyên suốt

✓ **Tài Liệu Toàn Diện**
- README chi tiết
- Tóm tắt triển khai
- Hướng dẫn biên dịch cho tất cả nền tảng
- Nhận xét mã và định dạng Doxygen

## 📄 Giấy Phép

Dự án này được cung cấp cho mục đích giáo dục và kiểm tra.

---

**Cập Nhật Lần Cuối**: 2026-03-24
**Tiêu Chuẩn C++**: C++17
**Yêu Cầu Trình Biên Dịch**: GCC 7+, Clang 5+, MSVC 2017+
**Tổng Cộng Dòng Mã**: 3000+
**Tài Liệu**: Toàn Diện

