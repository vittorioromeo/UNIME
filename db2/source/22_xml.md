



# XML

* **XML** *(extensible markup language)* is both a markup language and a meta-language to specify markup languages.

* A data model can be defined using **DTD** or **XSD**.

* Queries can be executed with **Xquery** or **XSL**.

* An XML document is **well-formed** when the syntax is valid.

* An XML document is **valid** when the contents respect a data model *(schema)*.

* Namespaces are handled by using prefixes.

## DTD

* Defining subelement occurrences:

    ```dtd
    <!ELEMENT product (description)>
    <!ELEMENT product (description?)>

    <!ELEMENT product_list (product+)>
    <!ELEMENT product_list (product*)>
    ```

* Attributes/modifiers:

    * `CDATA`: character data.

    * `ID`: identifier.

    * `IDREF`: this value is an ID of anoter element.

    * `ENTITY`: this value is an entity.

    * `NMTOKEN`: this value is a valid XML name.

* Constraints:

    * `#REQUIRED`.

    * `#IMPLIED`: can be absent.

    * `#FIXED "x"`: value needs to be `x`.

    * `#DEFAULT "x"`.


## XSD

* Another schema definition language.

* Compared to DTD:

    * More extensible and richer.

    * Can manage multiple namespaces.

    * Are XML themselves.


## XSL

* **Extensible stylesheet language**.

* Specifies how XML output is reprsented.

* **XSLT** *(XSL transformation)* transforms an XML in another XML or a different type *(like HTML)*.



## Xquery

* Can use **Xpath** expressions to query XML documents.

    * Examples:

        ```java
        doc("books.xml”)/List/Book 
        doc("books.xml”)/List/Book[Editore = ‘Bompiani’]/Title 
        doc("books.xml”)//Author 
        doc("books.xml”)/List/Book[2]/*
        ```

* Can use complex **Xquery** expressions combined with Xpath.

    * `FOR`, `LET`, `WHERE`, `ORDER BY`, `RETURN`, `INSERT`, `DELETE`.

    * Examples:

        ```java
        for $book in doc("books.xml”)//Book
        return $book
        ```

        ```java
        for $book in doc("books.xml”)//Book
        WHERE $book/Editor = “Bompiani” and $book/@availability = “S”
        return $book
        ```