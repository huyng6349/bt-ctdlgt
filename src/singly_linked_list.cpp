#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS

#include <cstddef>
#include <cstdint>

#include <stdio.h>
#include <inttypes.h>

//=============================================================
// Singly Linked List
//=============================================================

template <typename T>
struct Sl_Node {
    T data;
    Sl_Node<T>* next;

    explicit Sl_Node() : data(), next(nullptr) {
    }

    explicit Sl_Node(T data) : data(data), next(nullptr) {
    }

    auto push_next(T data) -> Sl_Node<T>* {
        auto node = new Sl_Node(data);
        return push_next(node);
    }

    auto push_next(Sl_Node<T>* node) -> Sl_Node<T>* {
        if (next != nullptr)
            node->next = next;
        next = node;
        return next;
    }

    [[nodiscard]] auto pop_next() -> Sl_Node<T>* {
        auto node = next;
        next = node->next;
        return node;
    }
};

template <typename T>
struct Sl_List {
    Sl_Node<T>* head;
    Sl_Node<T>* tail;

    explicit Sl_List() : head(nullptr), tail(nullptr) {
    }

    ~Sl_List() {
        clear();
    }

    auto clear() -> void {
        auto curr = head;
        while (curr != nullptr) {
            auto node = curr;
            curr = curr->next;
            delete node;
        }
    }

    auto is_empty() const -> bool {
        return head == nullptr;
    }

    auto count_nodes() const -> std::size_t {
        std::size_t result = 0;
        auto curr = head;
        while (curr != nullptr) {
            result += 1;
            curr = curr->next;
        }
        return result;
    }

    auto push_tail(T data) -> Sl_Node<T>* {
        auto node = new Sl_Node(data);
        return push_tail(node);
    }

    auto push_tail(Sl_Node<T>* node) -> Sl_Node<T>* {
        if (tail == nullptr) {
            tail = node;
            if (head == nullptr)
                head = node;
        }
        else {
            tail = tail->push_next(node);
        }
        return tail;
    }

    auto push_head(T data) -> Sl_Node<T>* {
        auto node = new Sl_Node(data);
        return push_head(node);
    }

    auto push_head(Sl_Node<T>* node) -> Sl_Node<T>* {
        if (head == nullptr) {
            head = node;
            if (tail == nullptr)
                tail = node;
        }
        else {
            node->push_next(head);
            head = node;
        }
        return head;
    }

    [[nodiscard]] auto pop_tail() -> Sl_Node<T>* {
        if (head == nullptr) [[unlikely]]
            return nullptr;

        if (head == tail) [[unlikely]] {
            auto node = tail;
            head = nullptr;
            tail = nullptr;
            return node;
        }

        auto curr = head;
        while (curr->next != tail)
            curr = curr->next;
        tail = curr;
        return curr->pop_next();
    }

    [[nodiscard]] auto pop_head() -> Sl_Node<T>* {
        if (head == nullptr) [[unlikely]]
            return nullptr;

        if (head == tail) [[unlikely]] {
            auto node = head;
            head = nullptr;
            tail = nullptr;
            return node;
        }

        auto curr = head;
        head = head->next;
        return curr;
    }

    auto find_data(T data) const -> Sl_Node<T>* {
        auto curr = head;
        while (curr != nullptr && curr->data != data)
            curr = curr->next;
        return curr;
    }
};

//=============================================================
// Main
//=============================================================

int main() {
    // Demo cách sử dụng
    auto list = Sl_List<int>();

    // *10*
    list.push_tail(10);
    // 10 -> *20*
    list.push_tail(20);
    // 10 -> 20 -> *30*
    list.push_tail(30);
    // *90* -> 10 -> 20 -> 30
    list.push_head(90);
    // *80* -> 90 -> 10 -> 20 -> 30
    list.push_head(80);

    // 80 -> 90 -> 10 -> *15* -> 20 -> 30
    list.find_data(10)->push_next(15);

    // 80 -> 90 -> 10 -> 15 -> 20 -> ~30~
    // 80 -> 90 -> 10 -> 15 -> 20
    auto node_30 = list.pop_tail();
    delete node_30;

    // 80 -> 90 -> *95* -> 10 -> 15 -> 20
    list.find_data(90)->push_next(95);

    // 80 -> 90 -> 95 -> ~10~ -> 15 -> 20
    // 80 -> 90 -> 95 -> 15 -> 20
    auto node_10 = list.find_data(95)->pop_next();
    delete node_10;

    // ~80~ -> 90 -> 95 -> 15 -> 20
    // 90 -> 95 -> 15 -> 20
    auto node_80 = list.pop_head();
    delete node_80;

    auto curr = list.head;
    while (curr != nullptr) {
        printf("%d -> ", curr->data);
        curr = curr->next;
    }
    printf("\n");
}
