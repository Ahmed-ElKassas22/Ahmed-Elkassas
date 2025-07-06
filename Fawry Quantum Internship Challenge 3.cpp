#include <iostream>
#include <string>
using namespace std;

// setting max number of products and max items allowed in the cart
const int MAX_PRODUCTS = 10;
const int MAX_CART_ITEMS = 20;

// product class with attributes
class Product {
public:
    string name;
    double price;
    int quantity;
    bool expirable;
    bool expired;
    bool shippable;
    double weight;

    Product() {}
    Product(string n, double p, int q, bool e = false, bool ex = false, bool s = false, double w = 0.0)
        : name(n), price(p), quantity(q), expirable(e), expired(ex), shippable(s), weight(w) {}
};

// each cart item stores a pointer to product and its quantity
struct CartItem {
    Product* product;
    int quantity;
};

// cart class to store cart items and their count
class Cart {
public:
    CartItem items[MAX_CART_ITEMS];
    int itemCount = 0;

    // adds a product to the cart with the given quantity
    void add(Product* product, int quantity) 
    {
        if (quantity <= 0 || quantity > product->quantity) 
        {
            cout << "invalid quantity for " << product->name << endl;
            return;
        }
        if (itemCount < MAX_CART_ITEMS) 
        {
            items[itemCount++] = {product, quantity};
        } 
        else 
        {
            cout << "cart is full.\n";
        }
    }

    // checks if the cart is empty
    bool isEmpty() const 
    {
        return itemCount == 0;
    }
};

// customer class to store balance
class Customer {
public:
    double balance;
    Customer(double b) : balance(b) {}
};

// function to perform checkout logic
void checkout(Customer& customer, Cart& cart) 
{
    if (cart.isEmpty()) 
    {
        cout << "error: cart is empty.\n";
        return;
    }

    double subtotal = 0, shipping = 0, totalWeight = 0;
    bool hasShippable = false;

    // print cart content and calculate totals
    cout << endl << "Shipment Content:" << endl;
    for (int i = 0; i < cart.itemCount; ++i) 
    {
        Product* p = cart.items[i].product;
        int qty = cart.items[i].quantity;

        if (p->expirable && p->expired) 
        {
            cout << "Error: " << p->name << " is expired.\n";
            return;
        }
        if (qty > p->quantity) 
        {
            cout << "Error: not enough stock for " << p->name << ".\n";
            return;
        }

        subtotal += qty * p->price;

        // handle shippable products
        if (p->shippable) 
        {
            cout << qty << "x " << p->name << " " << p->weight * qty << "g\n";
            totalWeight += (p->weight * qty) / 1000.0;
            shipping += 10;
            hasShippable = true;
        }

        // reduce product stock
        p->quantity -= qty;
    }

    double total = subtotal + shipping;
    if (customer.balance < total) 
    {
        cout << "Error: insufficient balance.\n";
        return;
    }

    // deduct amount from customer balance
    customer.balance -= total;

    // print shipment info if needed
    if (hasShippable) 
    {
        cout << "Total package weight " << totalWeight << "kg\n";
    }

    // print final receipt
    cout << "** Checkout receipt **\n";
    for (int i = 0; i < cart.itemCount; ++i) 
    {
        Product* p = cart.items[i].product;
        int qty = cart.items[i].quantity;
        cout << qty << "x " << p->name << " " << qty * p->price << "$"<<"\n";
    }
    cout << "----------------------\n";
    cout << "Subtotal " << subtotal << "$"<<"\n";
    cout << "Shipping " << shipping << "$"<<"\n";
    cout << "Total " << total << "$"<<"\n";
    cout << "Customer balance: " << customer.balance << "$"<<"\n";
}

int main() {
    // list of available products
    Product products[MAX_PRODUCTS] = {
        Product("Cheese", 15, 10, true, false, true, 150),
        Product("Biscuits", 10, 10, true, false, true, 100),
        Product("Tv", 300, 5, false, false, true, 8000),
        Product("Scratch card", 30, 40)
    };
    int productCount = 4;

    Customer customer(500);
    Cart cart;

    int choice, qty;
    while (true) 
    {
        // shows the option menu
        cout << "\nAvailable products:\n";
        for (int i = 0; i < productCount; ++i) 
        {
            cout << i + 1 << ". " << products[i].name << " - $" << products[i].price << " (Stock: " << products[i].quantity << ")\n";
        }
        cout << "0. Checkout\n";
        cout << "Enter product number to add to cart (or 0 to checkout): ";
        cin >> choice;

        if (choice == 0) break;
        if (choice < 1 || choice > productCount) 
        {
            cout << "invalid choice.\n";
            continue;
        }

        cout << "enter quantity: ";
        cin >> qty;
        cart.add(&products[choice - 1], qty);
    }

    // performs checkout
    checkout(customer, cart);
    return 0;
}
