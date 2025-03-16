
#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace vole::datamodel {

    class node_visitor;

    using num_type = double;
    using string_type = std::string;
    using bool_type = bool;
    class null_type {};

    /**
     * Abstract entity class to represent internal data nodes
     */
    class node {
    public:
        using shared_node = std::shared_ptr<node>;
        using node_list = std::vector<shared_node>;
        explicit node(std::string name) : _name(std::move(name)) {}
        virtual ~node() = default;
        [[nodiscard]] virtual std::string type() const = 0;

        [[nodiscard]] std::string_view name() const {
            return _name;
        }

        [[nodiscard]] std::string format_debug() const;
    
        virtual void apply(node_visitor &visitor) const = 0;
    
    private:
        std::string _name;
    };

    using shared_node = node::shared_node;
    using node_list = node::node_list;

    class array_node : public node {
    public:
        explicit array_node(std::string name)
            : node(std::move(name)) {}

        std::string type() const override;
        virtual void add_child(shared_node node);
        [[nodiscard]] shared_node get_child(size_t index) const;
        [[nodiscard]] const node_list& get_children() const;
        void apply(node_visitor &visitor) const override;
    private:
        node_list children;
    };

    using literal_value = std::variant<
        bool_type,
        null_type,
        num_type,
        string_type
    >;

    class literal_node : public node {
    public:
        explicit literal_node(std::string name, literal_value value)
            : node(std::move(name)), value(std::move(value)) {}

        [[nodiscard]] std::string type() const override;
        [[nodiscard]] std::string render() const;
        void apply(node_visitor &visitor) const override;
    protected:
        literal_value value;
    };

    class object_node : public node {
    public:
        explicit object_node(std::string name)
            : node(std::move(name)) {}

        [[nodiscard]] std::string type() const override;
        void add_child(shared_node node);
        [[nodiscard]] shared_node get_child(std::string_view name) const;
        [[nodiscard]] const node_list& get_children() const;
        void apply(node_visitor &visitor) const override;
    private:
        node_list children;
    };

    class node_visitor {
    public:
        virtual ~node_visitor() = default;
        virtual void visit(const array_node &node) = 0;
        virtual void visit(const literal_node &node) = 0;
        virtual void visit(const object_node &node) = 0;
    };
}
