#ifndef BINEXPR_H
#define BINEXPR_H

// Inspired by:
// https://gieseanw.wordpress.com/2019/10/20/we-dont-need-no-stinking-expression-templates/
// https://wandbox.org/permlink/0byiMsdCFgNw7Bq1

// C++17’s deduction guides saves the caller the hassle of specifying template arguments 
// (or us providing a trampoline function like make_binary_expression) when constructing a BinaryExpression
template<class T, class U, class Callable>
struct BinaryExpression
{

    private:
    const T* left = nullptr;
    const U* right = nullptr;
    Callable callable;

    public:
    template<class Func>
    BinaryExpression(const T& _left, const U& _right, Func&& _func) : left{&_left}, right{&_right}, callable{std::forward<Func>(_func)}
    {}
    
    auto operator()() const
    {
        return callable(*left, *right);
    }
    
    // Composing BinaryExpressions is made easy if we allow them 
    // to be implicitly converted to the same type as the result of invoking them like so:
    // This is eager, though!
    using result_t = decltype(std::declval<Callable>()(std::declval<const T&>(), std::declval<const U&>()));
    operator result_t() const
    {
        return this->operator()();
    }
    
	friend std::ostream& operator<<(std::ostream& os, const BinaryExpression<T,U,Callable>& be) {
    	os << be();
    	return os;
    }
};
template<class T, class U, class Func>
BinaryExpression(const T&, const U&, Func&&) -> BinaryExpression<T, U, Func>;

template<class T, class U>
auto operator+(T const& lhs, U const& rhs) {
    return BinaryExpression{
        lhs, rhs,
        [](auto const& l, auto const &r) {
            return l + r; }    
    };
}

#endif // ifndef BINEXPR_H