--
-- PostgreSQL database dump
--

-- Dumped from database version 12.17 (Ubuntu 12.17-0ubuntu0.20.04.1)
-- Dumped by pg_dump version 12.17 (Ubuntu 12.17-0ubuntu0.20.04.1)

-- Started on 2024-01-20 14:59:46 EST

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- TOC entry 202 (class 1259 OID 18510)
-- Name: jobStatusTypes; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."jobStatusTypes" (
    id integer NOT NULL,
    "createdAt" timestamp with time zone NOT NULL,
    "updatedAt" timestamp with time zone NOT NULL,
    name text NOT NULL
);


ALTER TABLE public."jobStatusTypes" OWNER TO postgres;

--
-- TOC entry 203 (class 1259 OID 18516)
-- Name: jobs; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.jobs (
    id integer NOT NULL,
    "createdAt" timestamp with time zone NOT NULL,
    "updatedAt" timestamp with time zone NOT NULL,
    "jobStatusTypeId" integer NOT NULL,
    "statusMessage" text,
    "userId" integer NOT NULL
);


ALTER TABLE public.jobs OWNER TO postgres;

--
-- TOC entry 204 (class 1259 OID 18522)
-- Name: outputTypes; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."outputTypes" (
    id integer NOT NULL,
    "createdAt" timestamp with time zone NOT NULL,
    "updatedAt" timestamp with time zone NOT NULL,
    name text NOT NULL
);


ALTER TABLE public."outputTypes" OWNER TO postgres;

--
-- TOC entry 205 (class 1259 OID 18528)
-- Name: outputs; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.outputs (
    id integer NOT NULL,
    "createdAt" timestamp with time zone NOT NULL,
    "updatedAt" timestamp with time zone NOT NULL,
    "jobId" integer NOT NULL,
    "outputTypeId" integer NOT NULL,
    name text NOT NULL,
    value text NOT NULL
);


ALTER TABLE public.outputs OWNER TO postgres;

--
-- TOC entry 206 (class 1259 OID 18534)
-- Name: users; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.users (
    id integer NOT NULL,
    "createdAt" timestamp with time zone NOT NULL,
    "updatedAt" timestamp with time zone NOT NULL,
    username text NOT NULL,
    "usernameNormal" text NOT NULL,
    "passwordHash" text NOT NULL,
    affiliation text NOT NULL,
    name text NOT NULL,
    email text NOT NULL,
    "recoveryToken" text,
    "recoveryTokenExpiration" date
);


ALTER TABLE public.users OWNER TO postgres;

--
-- TOC entry 2989 (class 0 OID 18510)
-- Dependencies: 202
-- Data for Name: jobStatusTypes; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."jobStatusTypes" (id, "createdAt", "updatedAt", name) FROM stdin;
1	2024-01-19 00:00:00-05	2024-01-19 00:00:00-05	SUCCESS
2	2024-01-19 00:00:00-05	2024-01-19 00:00:00-05	FAILED
3	2024-01-19 00:00:00-05	2024-01-19 00:00:00-05	RUNNING
4	2024-01-19 00:00:00-05	2024-01-19 00:00:00-05	IN QUEUE
\.


--
-- TOC entry 2990 (class 0 OID 18516)
-- Dependencies: 203
-- Data for Name: jobs; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.jobs (id, "createdAt", "updatedAt", "jobStatusTypeId", "statusMessage", "userId") FROM stdin;
\.


--
-- TOC entry 2991 (class 0 OID 18522)
-- Dependencies: 204
-- Data for Name: outputTypes; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."outputTypes" (id, "createdAt", "updatedAt", name) FROM stdin;
1	2024-01-19 00:00:00-05	2024-01-19 00:00:00-05	quid
2	2024-01-19 00:00:00-05	2024-01-19 00:00:00-05	bool
3	2024-01-19 00:00:00-05	2024-01-19 00:00:00-05	double
\.


--
-- TOC entry 2992 (class 0 OID 18528)
-- Dependencies: 205
-- Data for Name: outputs; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.outputs (id, "createdAt", "updatedAt", "jobId", "outputTypeId", name, value) FROM stdin;
\.


--
-- TOC entry 2993 (class 0 OID 18534)
-- Dependencies: 206
-- Data for Name: users; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.users (id, "createdAt", "updatedAt", username, "usernameNormal", "passwordHash", affiliation, name, email, "recoveryToken", "recoveryTokenExpiration") FROM stdin;
\.


--
-- TOC entry 2852 (class 2606 OID 18541)
-- Name: jobs jobs_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.jobs
    ADD CONSTRAINT jobs_pkey PRIMARY KEY (id);


--
-- TOC entry 2854 (class 2606 OID 18543)
-- Name: outputTypes outputTypes_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."outputTypes"
    ADD CONSTRAINT "outputTypes_pkey" PRIMARY KEY (id);


--
-- TOC entry 2856 (class 2606 OID 18545)
-- Name: outputs outputs_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.outputs
    ADD CONSTRAINT outputs_pkey PRIMARY KEY (id);


--
-- TOC entry 2850 (class 2606 OID 18547)
-- Name: jobStatusTypes statusTypes_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."jobStatusTypes"
    ADD CONSTRAINT "statusTypes_pkey" PRIMARY KEY (id);


--
-- TOC entry 2858 (class 2606 OID 18549)
-- Name: users users_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_pkey PRIMARY KEY (id);


--
-- TOC entry 2859 (class 2606 OID 18550)
-- Name: jobs jobs_jobstatustypeid_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.jobs
    ADD CONSTRAINT jobs_jobstatustypeid_fk FOREIGN KEY ("jobStatusTypeId") REFERENCES public."jobStatusTypes"(id) NOT VALID;


--
-- TOC entry 2860 (class 2606 OID 18555)
-- Name: jobs jobs_userid_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.jobs
    ADD CONSTRAINT jobs_userid_fk FOREIGN KEY ("userId") REFERENCES public.users(id) ON DELETE CASCADE NOT VALID;


--
-- TOC entry 2861 (class 2606 OID 18560)
-- Name: outputs outputs_jobid_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.outputs
    ADD CONSTRAINT outputs_jobid_fk FOREIGN KEY ("jobId") REFERENCES public.jobs(id) NOT VALID;


--
-- TOC entry 2862 (class 2606 OID 18565)
-- Name: outputs outputs_typeid_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.outputs
    ADD CONSTRAINT outputs_typeid_fk FOREIGN KEY ("outputTypeId") REFERENCES public."outputTypes"(id) NOT VALID;


-- Completed on 2024-01-20 14:59:46 EST

--
-- PostgreSQL database dump complete
--

