
---

### 1. C4 Diagrams (Software Architecture)

#### **1.1 System Context**
Describes the high-level ecosystem and external integrations.

```mermaid
C4Context
    title System Context for RouteOptimizer Data Pipeline
    
    Person(analyst, "Data Analyst", "Explores data via Superset and runs SQL queries in ClickHouse.")
    Person(dev, "Data Engineer", "Maintains ETL scripts and monitors the Spark/S3 infrastructure.")
    
    System_Boundary(lakehouse_platform, "RouteOptimizer Platform") {
        System(core_platform, "Lakehouse Stack", "Processes raw data into actionable insights.")
    }

    System_Ext(keycloak, "Keycloak (SSO)", "Identity Provider for OIDC authentication.")
    System_Ext(s3_sources, "External S3 / Kaggle", "Upstream data sources for Taxi and E-Commerce records.")

    Rel(analyst, core_platform, "Visualizes metrics, performs OLAP queries", "HTTPS/SQL")
    Rel(dev, core_platform, "Deploys ETL, manages storage", "CLI/Docker")
    Rel(core_platform, keycloak, "Delegates Auth", "OIDC/JWT")
    Rel(core_platform, s3_sources, "Pulls raw datasets", "S3 API")
```

#### **1.2 Container View**
Detailed view of the internal services and their roles.

```mermaid
C4Container
    title Container Diagram for RouteOptimizer
    
    Container_Boundary(frontend, "User Interface") {
        Container(webapp_ui, "Platform UI", "React", "Management console for data uploads and schema discovery.")
        Container(superset, "Superset", "Python", "BI dashboards connecting to ClickHouse.")
    }

    Container_Boundary(backend_services, "Processing & API") {
        Container(api, "WebApp Backend", "FastAPI", "Orchestrates schema discovery and manual ClickHouse ingestion.")
        Container(spark, "Spark ETL", "PySpark", "Batch jobs transforming Raw S3 into Iceberg tables.")
    }

    Container_Boundary(data_layer, "Storage & Query") {
        ContainerDb(ceph, "Ceph RGW / MinIO", "S3-Compatible", "Object store for Raw, Silver, and Iceberg data.")
        ContainerDb(clickhouse, "ClickHouse", "OLAP Engine", "Primary query engine with native Iceberg support.")
        ContainerDb(postgres, "Metadata DB", "PostgreSQL", "Stores user sessions, dataset schemas, and job history.")
    }

    Rel(webapp_ui, api, "API Calls", "JSON/HTTPS")
    Rel(api, ceph, "Generates Presigned URLs", "S3 SDK")
    Rel(api, clickhouse, "Ingests S3 data", "HTTP/SQL")
    Rel(api, postgres, "Persists Metadata", "SQL")
    
    Rel(spark, ceph, "Reads Raw / Writes Iceberg", "S3A/Iceberg")
    Rel(clickhouse, ceph, "Queries Iceberg tables", "Iceberg Engine")
    Rel(superset, clickhouse, "Fetches analytics", "SQL/HTTP")
```

#### **1.3 Component View: Ingestion Service**
A deep dive into how the WebApp Backend handles data ingestion.

```mermaid
C4Component
    title Component Diagram: WebApp Ingestion Service
    
    Component(schema_discovery, "Schema Discovery", "Python/Pandas", "Analyzes S3 file headers and infers ClickHouse types.")
    Component(s3_helper, "S3 Helper", "Boto3/S3FS", "Manages presigned URLs and object listing.")
    Component(ch_client, "ClickHouse Client", "Requests/SQL", "Handles DDL (CREATE TABLE) and DML (INSERT FROM S3).")
    Component(bg_worker, "Background Worker", "FastAPI Tasks", "Executes ingestion jobs asynchronously.")

    Rel(bg_worker, schema_discovery, "Uses", "Internal")
    Rel(bg_worker, ch_client, "Triggers Ingestion", "SQL")
    Rel(ch_client, s3_helper, "Requests URLs", "Internal")
```

---

### 2. Dataflow Diagrams (Lakehouse-Specific)

#### **2.1 Medallion Pipeline Flow**
The standard path for large-scale datasets (Taxi/E-Commerce).

```mermaid
graph LR
    subgraph "Raw Layer (Landing)"
        direction TB
        S3_Taxi[(s3://raw-taxi-data)]
        S3_Ecom[(s3://raw-ecommerce-data)]
    end

    subgraph "Spark Transformation"
        direction TB
        ETL_T[Taxi ETL Job]
        ETL_E[E-Com ETL Job]
    end

    subgraph "Iceberg Layer (Silver/Gold)"
        direction TB
        T_Daily[(Taxi Daily Stats)]
        E_Funnel[(E-Com Funnel)]
    end

    subgraph "Serving Layer"
        CH[ClickHouse Iceberg Engine]
        SS[Superset BI]
    end

    S3_Taxi --> ETL_T --> T_Daily
    S3_Ecom --> ETL_E --> E_Funnel
    
    T_Daily -.-> CH
    E_Funnel -.-> CH
    CH --> SS
```

---

### 3. Storage & Table Layout

#### **3.1 Iceberg Catalog Structure**
How data is organized in the `processed-data` bucket.

```mermaid
classDiagram
    class S3_Processed_Bucket {
        +warehouse/default/
    }
    class Iceberg_Table {
        +metadata/
        |-- v1.metadata.json
        |-- version-hint.text
        +data/
        |-- part-000.parquet
        |-- part-001.parquet
    }
    S3_Processed_Bucket *-- Iceberg_Table : "Contains"
```

---

### 4. Security & Access Diagram

#### **4.1 SSO & RBAC Architecture**
The authentication flow using Keycloak.

```mermaid
sequenceDiagram
    participant User
    participant WebApp
    participant Keycloak
    participant API
    participant ClickHouse

    User->>WebApp: Access Dashboard
    WebApp->>Keycloak: Redirect to Login
    Keycloak-->>User: Auth Challenge (Username/Password)
    User->>Keycloak: Submit Credentials
    Keycloak-->>WebApp: Authorization Code
    WebApp->>Keycloak: Exchange Code for JWT
    WebApp->>API: API Request + Bearer Token
    API->>Keycloak: Validate JWT Signature
    API->>ClickHouse: Execute Query (Scoped to User/Org)
    ClickHouse-->>API: Data Results
    API-->>WebApp: JSON Response
```

---

### 5. Infrastructure / Deployment

#### **5.1 Docker Compose Local Topology**
The current local setup as defined in `docker-compose.yml`.

```mermaid
graph TD
    subgraph "External Access"
        Browser[User Browser]
    end

    subgraph "Docker Network (Internal)"
        direction TB
        NGINX[WebApp / Proxy]
        API[FastAPI Backend]
        SS[Superset]
        CH[ClickHouse]
        SPK[Spark Master/Worker]
        CEPH[Ceph RGW]
        KC[Keycloak]
        PG[PostgreSQL]
    end

    Browser -->|8001| NGINX
    Browser -->|8088| SS
    Browser -->|8080| KC
    Browser -->|7480| CEPH
    
    API --> PG
    API --> CH
    API --> CEPH
    SPK --> CEPH
    CH --> CEPH
    SS --> CH
    SS --> KC
```

---

### Summary of Key Architectural Choices:
*   **Decoupled Storage**: Using Ceph RGW (S3) allows the architecture to scale from local Docker to AWS S3/Azure Blob without code changes.
*   **Engine-Agnostic Processing**: Spark handles the heavy lifting of ETL, while ClickHouse handles the high-concurrency query load.
*   **Iceberg Table Format**: Enables ACID transactions, schema evolution, and time travel, which are critical for a robust Lakehouse.
*   **Centralized IAM**: Keycloak ensures that security policies are consistent across the WebApp, Superset, and future services.
