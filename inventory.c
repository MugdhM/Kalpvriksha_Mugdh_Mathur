#include<stdio.h>
#include<stdlib.h>

#define NameLength 50
#define False 0
#define True 1

typedef struct product {
    int product_id;
    char name[NameLength];
    float price;
    int quantity;
    
} Product;

//global variables
int totalProducts;
Product *productList;



void addProduct() {

    productList = (Product *) realloc(productList,++totalProducts * sizeof(Product));

    
    printf("\n\nEnter Details for new Product :\n");
    printf("Product ID: ");
    scanf("%d",&productList[totalProducts-1].product_id);

    printf("Product Name: ");
    scanf("%s",&productList[totalProducts-1].name);

    printf("Product Price: ");
    scanf("%f",&productList[totalProducts-1].price);

    printf("Product Quantity: ");
    scanf("%d",&productList[totalProducts-1].quantity);

    printf("\n Product Added Successfully\n");
}

void printDetails(int index) {
    printf("\nProduct ID: %d | Name: %s | Price: %f | Quantinty: %d\n",productList[index].product_id,productList[index].name,productList[index].price,productList[index].quantity);
}

void showProducts() {
    printf("=============== PRODUCT LIST ===================");
    for(int i = 0; i<totalProducts;i++) {
        printDetails(i);
    }
}

void updateQuantity() {
    int id,newQuantity;
    printf("\n\nEnter Product ID to update quantity: ");
    scanf("%d",&id);

    printf("Enter new Quantity: ");
    scanf("%d",&newQuantity);

    for(int i = 0; i<totalProducts; i++) {
        if(productList[i].product_id == id) {
            productList[i].quantity = newQuantity;
        }
    }
}

void searchProductById() {
    int id;
    printf("\n\nEnter Product ID to Search: ");
    scanf("%d",&id);

    for(int i = 0; i<totalProducts; i++) {
        if(productList[i].product_id == id) {
            printDetails(i);
        }
    }
}

int matchNames(int index,char name[NameLength]) {
    int i = 0;
    while(productList[index].name[i] != '\0' && name[i] != '\0') {
        if(productList[index].name[i] != name[i]) return False;
    }

    if(name[i] == '\0') return True; else return False;
}

void searchProductByName() {
    char name[NameLength];
    
    printf("\n\nEnter name to search (partial allowed): ");
    scanf("%s",name);

    for(int i = 0; i<totalProducts; i++) {
        if(matchNames(i,name)) printDetails(i);
    }
}

void searchProductByPriceRange() {
    int minPrice = 0,maxPrice = 0;

    printf("\nEnter minimum price: ");
    scanf("%d",&minPrice);

    printf("Enter maximum price: ");
    scanf("%d",&maxPrice);

    printf("\n\nProducts in Range - \n");
    for(int i = 0; i<totalProducts;i++) 
        if(productList[i].price >= minPrice && productList[i].price <= maxPrice)
            printDetails(i);

}

void deleteProductById() {
    int id;

    printf("\n\nEnter Product ID to delete: ");
    scanf("%d",&id);

    for(int i = 0; i < totalProducts; i++) {
        if(productList[i].product_id == id) {


            printf("L");
            break;
        }
    }

    printf("\nProduct deleted successfully! ");
}


void displayMenu() {
    printf("\n========== INVENTORY MENU ============\n");
    printf("1. Add New Product\n");
    printf("2. View All Products\n");
    printf("3. Update Quantity\n");
    printf("4. Search Product by ID\n");
    printf("5. Search Product by Name\n");
    printf("6. Search Product by Price Range\n");
    printf("7. Delete Product\n");
    printf("8. Exit\n");
    printf("Enter Your Choice : ");
}

void Start() {
    int choice = 0;
    do {
        displayMenu();
        scanf("%d",&choice);
        
        switch(choice) {
            case 1: addProduct();
                break;
            case 2: showProducts();
                break;
            case 3: updateQuantity();
                break;
            case 4: searchProductById();
                break;
            case 5: searchProductByName();
                break;
            case 6: searchProductByPriceRange();
                break;
            case 7: deleteProductById();
                break;
        }

    } while(choice != 8);
}

int main() {
    totalProducts = 0;
    productList = NULL;


    printf("Enter the initial number of products : ");
    scanf("%d",&totalProducts);

    productList = (Product *) malloc(totalProducts * sizeof(Product));

    for(int i = 0; i<totalProducts; i++) {
        printf("\n Enter Details for Product %d\n\n",i+1);

        printf("Product ID: ");
        scanf("%d",&productList[i].product_id);

        printf("Product Name: ");
        scanf("%s",&productList[i].name);

        printf("Product Price: ");
        scanf("%f",&productList[i].price);

        printf("Product Quantity: ");
        scanf("%d",&productList[i].quantity);
    }

    Start();

    return 0;
}
