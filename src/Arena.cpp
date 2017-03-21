#include "Arena.hpp"

namespace PAPG {

Arena::Arena(size_t size)
    : vertices()
    , maxPriority(0)
{
    vertices.reserve(size);
    for (size_t id = 0; id < size; id++) {
        vertices.push_back(Vertex(id));
    }
}

bool Arena::resize(size_t size)
{
    if (vertices.size() < size) {
        vertices.reserve(size);
        for (size_t id = vertices.size(); id < size; id++) {
            vertices.push_back(Vertex(id));
        }
        return true;
    }
    return false;
}

bool Arena::setVertexOwner(size_t id, Player owner)
{
    if (id >= vertices.size()) {
        return false;
    }

    vertices[id].owner = owner;

    return true;
}

bool Arena::setVertexPriority(size_t id, size_t priority)
{
    if (id >= vertices.size()) {
        return false;
    }

    vertices[id].priority = priority;

    if (priority > maxPriority) {
        maxPriority = priority;
    }

    return true;
}

bool Arena::setVertexLabel(size_t id, std::string label)
{
    if (id >= vertices.size()) {
        return false;
    }

    vertices[id].label = label;

    return true;
}

bool Arena::addEdge(size_t from, size_t to){
    if(from >= vertices.size() || to >= vertices.size()){
        return false;
    }

    // TODO check for duplicates
    vertices[from].outgoing.push_back(to);
    vertices[to].incoming.push_back(from);
    return true;
}


size_t Arena::countEvenOwnedVertices() const
{
    return std::count_if(vertices.begin(), vertices.end(), [](const Vertex& v) { return v.isOwnerEven(); });
}

size_t Arena::countOddOwnedVertices() const
{
    return std::count_if(vertices.begin(), vertices.end(), [](const Vertex& v) { return v.isOwnerOdd(); });
}

std::vector<size_t> Arena::getEvenOwnedVertices() const
{
    // TODO implement
    return std::vector<size_t>(0);
}
std::vector<size_t> Arena::getOddOwnedVertices() const
{
    // TODO implement
    return std::vector<size_t>(0);
}

size_t Arena::countVerticesByPriority(size_t priority) const
{
    return std::count_if(vertices.begin(), vertices.end(), [priority](const Vertex& v) { return v.priority == priority; });
}

} // PAPG