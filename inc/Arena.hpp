#ifndef ARENA_HPP
#define ARENA_HPP

#include <algorithm>
#include <string>
#include <vector>

namespace PAPG {
enum class Player {
    even,
    odd
};

struct Vertex {
    size_t id = 0;
    Player owner = Player::even;
    size_t priority = 0;
    std::vector<size_t> incoming;
    std::vector<size_t> outgoing;
    std::string label = "";

    explicit Vertex(size_t id)
        : id(id)
    {
    }

    Vertex(size_t id, Player owner, size_t priority, const std::string& label = "")
        : id(id)
        , owner(owner)
        , priority(priority)
        , label(label)
    {
    }

    Vertex(const Vertex& other)
        : id(other.id)
        , owner(other.owner)
        , priority(other.priority)
        , incoming(other.incoming)
        , outgoing(other.outgoing)
        , label(other.label)
    {
    }

    inline Vertex & operator=(const Vertex & rhs){
        this->id = rhs.id;
        this->owner = rhs.owner;
        this->priority = rhs.priority;
        this->incoming = rhs.incoming;
        this->outgoing = rhs.outgoing;
        this->label = rhs.label;

        return *this;
    }

    inline bool operator==(const Vertex& other) const
    {
        return (this == &other) // if pointers are equal, objects are too
            || (this->owner == other.owner
                   && this->priority == other.priority
                   && std::equal(this->incoming.begin(), this->incoming.end(), other.incoming.begin())
                   && std::equal(this->outgoing.begin(), this->outgoing.end(), other.outgoing.begin())
                   && this->label == other.label);
    }
    inline bool operator!=(const Vertex& other) const
    {
        return !(*this == other);
    }

    inline bool isPriorityEven() const { return !(priority % 2); }
    inline bool isPriorityOdd() const { return !isPriorityEven(); }
    inline bool isOwnerEven() const { return owner == Player::even; }
    inline bool isOwnerOdd() const { return owner == Player::odd; }
};

class Arena {
private:
    std::vector<Vertex> vertices;
    size_t maxPriority = 0;

public:
    Arena() = delete; // always needs a size

    explicit Arena(size_t size);

    bool resize(size_t size);

    bool setVertexOwner(size_t id, Player owner);
    bool setVertexPriority(size_t id, size_t priority);
    bool setVertexLabel(size_t id, std::string label);

    bool addEdge(size_t from, size_t to);

    inline const Vertex& operator[](size_t id) const { return vertices[id]; }
    inline const std::vector<Vertex> & getVertices() const { return vertices; }
    
    inline size_t getSize() const { return vertices.size(); }
    inline size_t getMaxPriority() const { return maxPriority; }

    inline bool clearVertex(size_t id)
    {
        if (id < vertices.size()) {
            vertices[id] = Vertex(id);
            return true;
        }
        return false;
    }

    size_t countVerticesByPriority(size_t priority) const;
};

} // PAPG

#endif // ARENA_HPP
