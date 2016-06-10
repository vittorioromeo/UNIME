



# XML

* **XML** *(extensible markup language)* is both a markup language and a meta-language to specify markup languages.

* A data model can be defined using **DTD** or **XSD**.

* Queries can be executed with **Xquery** or **XSL**.

* An XML document is **well-formed** when the syntax is valid.

* An XML document is **valid** when the contents respect a data model *(schema)*.

* Namespaces are handled by using prefixes.

## DTD (Document Type Definition)

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


## XSD (XML Schema Definition)

* Another schema definition language.

* Compared to DTD:

    * More extensible and richer.

    * Can manage multiple namespaces.

    * XSD is valid XML.


### Example

```xml
<xs:element name="Attributo" type="xs:string">
    <xs:attribute name="lang" type="xs:string" 
        use="required"/>
</xs:element>

<xs:element name="age">
    <xs:simpleType>
        <xs:restriction base="xs:integer">
            <xs:minInclusive value="0"/>
            <xs:maxInclusive value="120"/>
        </xs:restriction>
    </xs:simpleType>
</xs:element>

<xs:element name="car">
    <xs:simpleType>
        <xs:restriction base="xs:string">
            <xs:enumeration value="Audi"/>
            <xs:enumeration value="Golf"/>
            <xs:enumeration value="BMW"/>
        </xs:restriction>
    </xs:simpleType>
</xs:element>

<xs:complexType name="tipoComplessoMio">
    <xs:sequence>
        <xs:element name="firstname" type="xs:string" 
            minOccurs="0" maxOccurs= "2"/>
        <xs:element name="lastname" type="xs:string" 
            minOccurs="2"/>
    </xs:sequence>
</xs:complexType>
<xs:element name="employee" type="tipoComplessoMio"/>

<xs:complexType name="tipoComplessoMioESTESO">
    <xs:complexContent>
        <xs:extension base="tipoComplessoMio">
            <xs:sequence>
                <xs:element name="address" type="xs:string"/>
                <xs:element name="city" type="xs:string"/>
                <xs:element name="country" type="xs:integer"/>
            </xs:sequence>
        </xs:extension>
    </xs:complexContent>
</xs:complexType>
<xs:element name="amministratore" type="tipoComplessoMioESTESO"/>

<xs:group name="custGroup">
    <xs:sequence> 
        <xs:element name="customer" type="xs:string"/>
        <xs:element name="orderdetails" type="xs:string"/>
    </xs:sequence>
</xs:group>

<xs:complexType name="ordertype"> Riuso di “custGroup”
    <xs:group ref="custGroup"/>
    <xs:attribute name="status" type="xs:string"/>
</xs:complexType>
<xs:element name="esempioGRUPPO" type="ordertype"/>
```


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