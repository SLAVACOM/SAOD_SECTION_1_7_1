#include <iostream>
#include <string>

using namespace std;

// ==== Структура потомка ====
struct ChildNode {
    int childId;
    ChildNode* next;
};

// ==== Структура узла дерева ====
struct Node {
    int id;
    int parentId;
    ChildNode* children;  // указатель на список потомков
    Node* next;           // следующий узел в списке всех узлов дерева
};

// ==== Структура дерева ====
struct Tree {
    Node* head;  // указатель на первый узел (корень)

    Tree() {
        // создаём корень с id = 0
        head = new Node{ 0, -1, nullptr, nullptr };
    }

    // Поиск узла по ID
    Node* findNode(int id) {
        Node* current = head;
        while (current) {
            if (current->id == id)
                return current;
            current = current->next;
        }
        return nullptr;
    }

    // Добавление новой вершины
    void addVertex(int parentId, int newId) {
        if (findNode(newId)) {
            cout << "Вершина с ID " << newId << " уже существует.\n";
            return;
        }

        Node* parent = findNode(parentId);
        if (!parent) {
            cout << "Родитель с ID " << parentId << " не найден.\n";
            return;
        }

        // создаём новую вершину
        Node* newNode = new Node{ newId, parentId, nullptr, nullptr };

        // вставим в линейный список узлов (в начало)
        newNode->next = head;
        head = newNode;

        // добавим как потомка родителю
        ChildNode* newChild = new ChildNode{ newId, parent->children };
        parent->children = newChild;

        cout << "✅ Добавлена вершина " << newId << " к родителю " << parentId << ".\n";
    }

    // Удаление поддерева
    void deleteSubtree(int id) {
        Node* toDelete = findNode(id);
        if (!toDelete) return;

        // удаляем всех потомков рекурсивно
        ChildNode* child = toDelete->children;
        while (child) {
            deleteSubtree(child->childId);
            child = child->next;
        }

        // удалим потомков
        child = toDelete->children;
        while (child) {
            ChildNode* next = child->next;
            delete child;
            child = next;
        }

        // удалить из списка узлов
        Node* prev = nullptr;
        Node* curr = head;
        while (curr) {
            if (curr->id == id) {
                if (prev)
                    prev->next = curr->next;
                else
                    head = curr->next;
                delete curr;
                break;
            }
            prev = curr;
            curr = curr->next;
        }

        // удалить из списка потомков у родителя
        Node* parent = findNode(toDelete->parentId);
        if (parent) {
            ChildNode* c = parent->children;
            ChildNode* prevChild = nullptr;
            while (c) {
                if (c->childId == id) {
                    if (prevChild)
                        prevChild->next = c->next;
                    else
                        parent->children = c->next;
                    delete c;
                    break;
                }
                prevChild = c;
                c = c->next;
            }
        }

        cout << "🗑️ Удалена вершина " << id << " и её поддерево.\n";
    }

    // Печать дерева
    void printTree() {
        cout << "📌 Текущие вершины дерева:\n";
        Node* current = head;
        while (current) {
            cout << "Вершина: " << current->id
                << ", Родитель: " << (current->parentId == -1 ? "None" : to_string(current->parentId)) << "\n";
            current = current->next;
        }
    }

    // Поиск вершины
    void findVertex(int id) {
        Node* node = findNode(id);
        if (node) {
            cout << "🔍 Найдена вершина " << id << ". Родитель: "
                << (node->parentId == -1 ? "None" : to_string(node->parentId)) << "\n";
        }
        else {
            cout << "❌ Вершина " << id << " не найдена.\n";
        }
    }
};

// === Главная программа ===
int main() {
    Tree tree;
    int choice, parentId, newId, id;

    setlocale(LC_ALL, "Russian"); // Установка локали для русского языка

    while (true) {
        cout << "\nМеню:\n";
        cout << "1. Добавить вершину\n";
        cout << "2. Удалить вершину\n";
        cout << "3. Показать дерево\n";
        cout << "4. Найти вершину\n";
        cout << "5. Выход\n";
        cout << "Выбор: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Введите ID родителя и новой вершины: ";
            cin >> parentId >> newId;
            tree.addVertex(parentId, newId);
            break;
        case 2:
            cout << "Введите ID вершины для удаления: ";
            cin >> id;
            if (id == 0) {
                cout << "⚠️ Нельзя удалить корень дерева.\n";
                break;
            }
            tree.deleteSubtree(id);
            break;
        case 3:
            tree.printTree();
            break;
        case 4:
            cout << "Введите ID вершины для поиска: ";
            cin >> id;
            tree.findVertex(id);
            break;
        case 5:
            cout << "Выход...\n";
            return 0;
        default:
            cout << "❗ Неверный выбор.\n";
        }
    }
}
