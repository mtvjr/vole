
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
    using null_type = nullptr_t;

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

    template <typename... Args>
    std::shared_ptr<array_node> make_array(Args&&... args) {
        return std::make_shared<array_node>(std::forward<Args>(args)...);
    }

    template <typename... Args>
    std::shared_ptr<literal_node> make_literal(Args&&... args) {
        return std::make_shared<literal_node>(std::forward<Args>(args)...);
    }

    template <typename... Args>
    std::shared_ptr<object_node> make_object(Args&&... args) {
        return std::make_shared<object_node>(std::forward<Args>(args)...);
    }

    /**
     * The node_visitor class offers an API to run a class-specific
     * function definition against a given node.
     *
     * Inherit from this class and define the type-specific visit
     * callbacks, then call node.apply(visitor). The appropriate
     * visit function for the given node shall be called.
     */
    class node_visitor {
        friend class literal_node;
        friend class object_node;
        friend class array_node;
    public:
        virtual ~node_visitor() = default;
    protected:
        virtual void visit(const array_node &node) {}
        virtual void visit(const literal_node &node) {}
        virtual void visit(const object_node &node) {}
    };

    /**
     * The node_descender class expands the capabilities of the
     * node_visitor class by offering a recursive descent view
     * into the node-tree. Instead of defining the 'visit' functions,
     * the caller should overload the 'on_enter' and 'on_exit' functions.
     */
    class node_descender : public node_visitor {
    public:
        ~node_descender() override = default;

    protected:
        void visit(const array_node &node) final;
        void visit(const literal_node &node) final;
        void visit(const object_node &node) final;
        virtual void on_enter(const array_node &node) {}
        virtual void on_exit(const array_node &node) {}
        virtual void on_enter(const literal_node &node) {}
        virtual void on_exit(const literal_node &node) {}
        virtual void on_enter(const object_node &node) {}
        virtual void on_exit(const object_node &node) {}

        /**
         * get_depth gives the caller the current level of the node tree
         * that the descender is on. Depth is update before and after visiting
         * children of a node.
         * @return the depth of the current node from the starter node
         */
        [[nodiscard]] size_t get_depth() const;
    private:
        size_t depth = 0;
    };

}
